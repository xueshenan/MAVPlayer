#include "ffmpeg_demuxer/ffmpeg_demuxer.h"

#include "base/log.h"
#include "base/unused.h"
#include "media_base/compressed_frame.h"

namespace media_base {

//static inline bool H264FourByteStartCode(const char *buf);

MediaDemuxer *CreateFFmpegDemuxer() {
    media_demuxer::FFmpegDemuxer *demuxer = new media_demuxer::FFmpegDemuxer();
    return demuxer;
}

}  // namespace media_base

namespace media_demuxer {

const AVRational gGlobalTimeBase = {1, AV_TIME_BASE};

void ffmpeg_log(void *avcl, int level, const char *fmt, va_list vl);

FFmpegDemuxer::FFmpegDemuxer() {
    _format_context = NULL;
    init_ffmpeg();
}

FFmpegDemuxer::~FFmpegDemuxer() {
    close();
}

bool FFmpegDemuxer::open(const std::string &file_path) {
    close();
    AVDictionary *dictionary = NULL;
    av_dict_set(&dictionary, "protocol_whitelist", "file,udp,rtp", 0);
    av_dict_set(&dictionary, "probesize", "32", 0);
    // av_dict_set(&dictionary, "fflags", "nobuffer", 0);                    //AVFMT_FLAG_NOBUFFER
    // av_dict_set(&dictionary, "fflags", "flush_packets", AV_DICT_APPEND);  //AVFMT_FLAG_FLUSH_PACKETS

    int ret = avformat_open_input(&_format_context, file_path.c_str(), NULL, &dictionary);
    if (ret != 0) {
        base::LogError() << "Open input failed";
        return false;
    }

    _format_context->flags = AVFMT_FLAG_NOBUFFER | AVFMT_FLAG_FLUSH_PACKETS;
    avformat_find_stream_info(_format_context, NULL);
    build_movie_info(file_path);

    _eof = false;
    _got_key_frame = false;
    return true;
}

void FFmpegDemuxer::close() {
    if (_format_context != NULL) {
        avformat_close_input(&_format_context);
        _format_context = NULL;
    }
}

media_base::MovieInfo *FFmpegDemuxer::movie_info() {
    return &_movie_info;
}

media_base::CompressedFrame *FFmpegDemuxer::read_frame() {
    AVPacket package;
    do {
        int ret = av_read_frame(_format_context, &package);
        if (ret < 0) {
            if (AVERROR(EAGAIN) == ret) {
                continue;
            } else {
                _eof = true;
                break;
            }
        } else {
            if (!_got_key_frame) {
                if (package.flags & AV_PKT_FLAG_KEY) {
                    _got_key_frame = true;
                } else {
                    continue;
                }
            }
        }
    } while (0);

    // TODO magic number
    if (package.size > 20) {
        media_base::CompressedFrame *compressed_frame = new media_base::CompressedFrame();
        compressed_frame->type = media_base::StreamType::StreamTypeVideo;
        compressed_frame->key_frame = package.flags & AV_PKT_FLAG_KEY;
        //TODO need convert timestamp
        compressed_frame->pts = package.pts;
        compressed_frame->dts = package.dts;
        compressed_frame->position = package.pos;
        compressed_frame->duration = package.duration;
        compressed_frame->frame_data_size = package.size;
        compressed_frame->frame_data = (int8_t *)malloc(package.size);
        memcpy(compressed_frame->frame_data, package.data, package.size);
        av_packet_unref(&package);
        return compressed_frame;
    }
    return NULL;
}

int64_t FFmpegDemuxer::current_timestamp() {
    //not implement
    return 0;
}

bool FFmpegDemuxer::eof() {
    //not implement
    return false;
}

void FFmpegDemuxer::init_ffmpeg() {
    av_log_set_callback(ffmpeg_log);
    avformat_network_init();
}

void FFmpegDemuxer::build_movie_info(const std::string &file_path) {
    _movie_info.name = file_path;
    _movie_info.format = _format_context->iformat->name;

    ///< store movie meta data
    if (_format_context->metadata != NULL) {
        AVDictionaryEntry *t = NULL;
        t = av_dict_get(_format_context->metadata, "", t, AV_DICT_IGNORE_SUFFIX);
        while (t != NULL) {
            if (t->key != NULL && t->value != NULL) {
                _movie_info.metadata[std::string(t->key)] = std::string(t->value);
            }
            t = av_dict_get(_format_context->metadata, "", t, AV_DICT_IGNORE_SUFFIX);
        }
    }

    ///< build stream info
    for (uint32_t i = 0; i < _format_context->nb_streams; i++) {
        AVStream *av_stream = _format_context->streams[i];
        media_base::StreamInfo *stream_info = new media_base::StreamInfo();
        stream_info->id = av_stream->id;

        AVDictionaryEntry *tag = av_dict_get(av_stream->metadata, "language", NULL, 0);
        if (tag != NULL && tag->value != NULL) {
            stream_info->language = tag->value;
        } else {
            stream_info->language = "eng";
        }

        if (av_stream->duration != AV_NOPTS_VALUE) {
            int64_t duration =
                av_rescale_q(av_stream->duration, av_stream->time_base, gGlobalTimeBase);
            if (duration > 0) {
                stream_info->duration = duration / 1000;
            }
        }

        AVCodecParameters *codec_param = av_stream->codecpar;
        if (codec_param == NULL) {
            stream_info->type = media_base::StreamType::StreamTypeUnknown;
            stream_info->codec_id = media_base::CodecID::CodecIDNone;
            _movie_info.streams.push_back(stream_info);
            continue;
        }

        else if (codec_param->codec_type == AVMEDIA_TYPE_VIDEO) {
            stream_info->type = media_base::StreamType::StreamTypeVideo;
            stream_info->width = codec_param->width;
            stream_info->height = codec_param->height;
            stream_info->pixel_format =
                FFMpegPixelFormatToMediaPixelFormat((AVPixelFormat)codec_param->format);

            ///< get video frame rate
            stream_info->framerate.numerator = av_stream->time_base.num;
            stream_info->framerate.denominator = av_stream->time_base.den;
            if (!is_valid_framerate(stream_info->framerate.numerator,
                                    stream_info->framerate.denominator)) {
                ///< use 30 as default framerate
                stream_info->framerate.numerator = 30;
                stream_info->framerate.denominator = 1;
            }

            if (codec_param->sample_aspect_ratio.num == 0) {
                stream_info->pixel_aspect.numerator = 1;
                stream_info->pixel_aspect.denominator = 1;
            } else {
                stream_info->pixel_aspect.numerator = codec_param->sample_aspect_ratio.num;
                stream_info->pixel_aspect.denominator = codec_param->sample_aspect_ratio.den;
            }

            stream_info->codec_tag = codec_param->codec_tag;
            stream_info->bits_per_coded_sample = codec_param->bits_per_coded_sample;
        } else if (codec_param->codec_type == AVMEDIA_TYPE_AUDIO) {
            stream_info->type = media_base::StreamType::StreamTypeAudio;

            stream_info->channels = codec_param->ch_layout.nb_channels;
            stream_info->samplerate = codec_param->sample_rate;
            stream_info->sample_format =
                FFMpegSampleFormatToMediaSampleFormat((AVSampleFormat)codec_param->format);
            stream_info->frame_size = codec_param->frame_size;
        } else if (codec_param->codec_type == AVMEDIA_TYPE_SUBTITLE) {
            stream_info->type = media_base::StreamType::StreamTypeSubtitle;
        } else {
            stream_info->type = media_base::StreamType::StreamTypeUnknown;
        }

        ///< common set media value
        stream_info->bitrate = codec_param->bit_rate;
        if (codec_param->codec_id != AV_CODEC_ID_NONE &&
            codec_param->codec_id != AV_CODEC_ID_PROBE) {
            const AVCodec *codec = avcodec_find_decoder(codec_param->codec_id);
            if (codec != NULL) {
                stream_info->codec_name = codec->name;
            }
        } else {  //unsupport codec id
            stream_info->type = media_base::StreamType::StreamTypeUnknown;
        }

        ///< stream metadata
        if (av_stream->metadata != NULL) {
            AVDictionaryEntry *t = NULL;
            t = av_dict_get(av_stream->metadata, "", t, AV_DICT_IGNORE_SUFFIX);
            while (t != NULL) {
                if (t->key != NULL && t->value != NULL) {
                    stream_info->metadata[t->key] = t->value;
                }
                t = av_dict_get(av_stream->metadata, "", t, AV_DICT_IGNORE_SUFFIX);
            }
        }

        ///< stream extra data
        if (codec_param->extradata != NULL && codec_param->extradata_size > 0) {
            stream_info->extra_data_size = codec_param->extradata_size;
            stream_info->extra_data = (uint8_t *)malloc(codec_param->extradata_size);
            memcpy(stream_info->extra_data, codec_param->extradata, codec_param->extradata_size);
        }

        stream_info->codec_id = FFMpegCodecIDToMeidaCodecID(codec_param->codec_id);
        _movie_info.streams.push_back(stream_info);
    }
}

bool FFmpegDemuxer::is_valid_framerate(int32_t numerator, int32_t denominator) {
    double framerateValue = 1.0 * numerator / denominator;
    if (framerateValue >= 360 || framerateValue < 1) {
        return false;
    } else {
        return true;
    }
}

//////////////////////////////////////////////////////////////////////////
//ffmepg log call back
void ffmpeg_log(void *avcl, int level, const char *fmt, va_list vl) {
    UNUSED(avcl);
    char buffer[256];
    vsnprintf(buffer, 256, fmt, vl);
    if (level == AV_LOG_WARNING) {
        base::LogWarn() << buffer;
    } else if (level == AV_LOG_ERROR) {
        base::LogError() << buffer;
    } else if (level == AV_LOG_FATAL) {
        base::LogError() << buffer;
    } else if (level == AV_LOG_TRACE || level == AV_LOG_DEBUG) {
        // base::LogDebug() << buffer;
    }
}

//////////////// H264 Help function ////////////////////////
// static inline bool H264FourByteStartCode(const char *buf) {
//     if (buf[0] == 0x00 && buf[1] == 0x00 && buf[2] == 0x00 && buf[3] == 0x01) {
//         return true;
//     }
//     return false;
// }

}  // namespace media_demuxer
