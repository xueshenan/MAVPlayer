#include "ffmpeg_decoder.h"

#include "base/log.h"

namespace media_decoder {

static __inline bool sup_threads_dec_slice(int x) {
    return x == AV_CODEC_ID_MPEG1VIDEO || x == AV_CODEC_ID_MPEG2VIDEO || x == AV_CODEC_ID_FFV1 ||
           x == AV_CODEC_ID_DVVIDEO;
}

static __inline bool sup_threads_dec_frame(int x) {
    return (x == AV_CODEC_ID_H264) || (x == AV_CODEC_ID_H265);
}

bool FFmpegDecoder::open_codec(media_base::AVCodecParam *codec_param) {
    if (codec_param == nullptr) {
        base::LogError() << "codec param is empty";
        return false;
    }

    enum AVCodecID ff_codec_id = MediaCodecIDToFFMpegCodecID(_codec_info.codec_id);
    _avcodec = avcodec_find_decoder(ff_codec_id);
    if (_avcodec == NULL) {
        base::LogError() << "Cannot find decoder";
        return false;
    }

    _avcodec_context = avcodec_alloc_context3(_avcodec);
    if (codec_param->extra_data_isze > 0 && codec_param->extra_data != NULL) {
        _extra_data_size = codec_param->extra_data_isze;
        _extra_data = (uint8_t *)malloc(_extra_data_size);
        memcpy(_extra_data, codec_param->extra_data, codec_param->extra_data_isze);
    }

    if (_extra_data_size > 0 && _extra_data != NULL) {
        _avcodec_context->extradata = _extra_data;
        _avcodec_context->extradata_size = _extra_data_size;
    } else {
        _avcodec_context->extradata = NULL;
        _avcodec_context->extradata_size = 0;
    }

    if (_codec_info.type == media_base::DecoderType::DecoderTypeVideo) {
        _avcodec_context->width = codec_param->width;
        _avcodec_context->height = codec_param->height;
        _avcodec_context->bits_per_coded_sample = codec_param->bits_per_coded_sample;

        uint32_t num_threads = 1;
        int thread_type = 0;
        if (codec_param->num_threads > 0) {
            num_threads = codec_param->num_threads;
        }

        if (num_threads > 1 && sup_threads_dec_frame(ff_codec_id)) {
            thread_type = FF_THREAD_FRAME;
        } else if (num_threads > 1 && sup_threads_dec_slice(ff_codec_id)) {
            thread_type = FF_THREAD_SLICE;
        }

        if (num_threads > 1 && thread_type != 0) {
        } else {
            num_threads = 1;
        }

        _avcodec_context->thread_type = thread_type;
        _avcodec_context->thread_count = num_threads;
        base::LogInfo() << "decoder thread count : " << num_threads;
        _avcodec_context->codec_tag = codec_param->codec_tag;

    } else if (_codec_info.type == media_base::DecoderType::DecoderTypeAudio) {
    }

    AVDictionary *opts = NULL;
    av_dict_set(&opts, "flags", "low_delay", 0);
    int ret = avcodec_open2(_avcodec_context, _avcodec, &opts);
    if (ret < 0) {
        base::LogError() << "Cannot open decoder";
        return false;
    }
    return true;
}

media_base::RawVideoFrame *FFmpegDecoder::decode_video_frame(
    media_base::CompressedFrame *compressed_frame) {
    ///<CompressedFrame to AVPacket
    uint32_t size = 0;
    uint8_t *pData = NULL;
    int64_t dts = -1;
    int64_t pts = -1;
    int flags = 0;
    int64_t pos = -1;

    if (compressed_frame != nullptr) {
        size = compressed_frame->frame_data_size;
        if (size < 2) {
            base::LogWarn() << "frame size too small";
            return nullptr;
        }
        pData = (uint8_t *)compressed_frame->frame_data;
        dts = compressed_frame->dts;
        pts = compressed_frame->pts;
        flags = compressed_frame->key_frame;
        pos = compressed_frame->position;

    } else {  //Flush decoder
    }

    AVPacket avpkt;
    memset(&avpkt, 0, sizeof(avpkt));
    avpkt.data = pData;
    avpkt.size = size;
    avpkt.dts = dts;
    avpkt.pts = pts;
    avpkt.flags = flags;
    avpkt.pos = pos;

    AVFrame *decoded_frame = av_frame_alloc();
    int ret = avcodec_send_packet(_avcodec_context, &avpkt);
    if (ret != 0) {
        base::LogError() << "send video packet failed";
        av_frame_unref(decoded_frame);
        return nullptr;
    }
    ret = avcodec_receive_frame(_avcodec_context, decoded_frame);
    if (ret != 0) {
        if (ret == AVERROR(EAGAIN)) {
            av_frame_unref(decoded_frame);
            return nullptr;
        } else {
            base::LogError() << "cannot decoder video, " << ret;
            av_frame_unref(decoded_frame);
            return nullptr;
        }
    }
    media_base::RawVideoFrame *raw_video_frame = new media_base::RawVideoFrame(
        FFMpegPixelFormatToMediaPixelFormat(_avcodec_context->pix_fmt), decoded_frame->width,
        decoded_frame->height);

    for (uint32_t i = 0; i < AV_NUM_DATA_POINTERS; i++) {
        if (decoded_frame->linesize[i] > 0) {
            raw_video_frame->PushFrameData(decoded_frame->linesize[i],
                                           (int8_t *)decoded_frame->data[i]);
        } else {
            break;
        }
    }
    av_frame_free(&decoded_frame);

    return raw_video_frame;
}

FFmpegDecoder::FFmpegDecoder(FFmpegCodecInfo *codec_info) {
    _codec_info = *codec_info;
    _extra_data = NULL;
    _extra_data_size = 0;
}

FFmpegDecoder::~FFmpegDecoder() {
    if (_extra_data_size > 0 && _extra_data != NULL) {
        free(_extra_data);
    }

    if (_avcodec_context != NULL) {
        avcodec_free_context(&_avcodec_context);
        _avcodec_context = NULL;
    }
}

}  // namespace media_decoder