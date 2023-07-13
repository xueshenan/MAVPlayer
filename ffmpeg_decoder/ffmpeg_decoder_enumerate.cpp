#include "ffmpeg_common.h"
#include "ffmpeg_decoder.h"
#include "ffmpeg_decoder_enumerator.h"

namespace media_decoder {

FFmpegDecoderEnumerator::FFmpegDecoderEnumerator() {}

FFmpegDecoderEnumerator::~FFmpegDecoderEnumerator() {
    for (uint32_t i = 0; i < _codec_infos.size(); i++) {
        FFmpegCodecInfo *codec_info = _codec_infos[i];
        delete codec_info;
        _codec_infos[i] = nullptr;
    }
    _codec_infos.clear();
}

void FFmpegDecoderEnumerator::init() {
    AVCodec *av_codec = NULL;
    do {
        //av_codec = av_codec_next(av_codec);
        if (av_codec != NULL && av_codec_is_decoder(av_codec) != 0) {
            enum AVMediaType media_type = avcodec_get_type(av_codec->id);
            media_base::DecoderType decoder_type = media_base::DecoderType::DecoderTypeNone;
            switch (media_type) {
                case AVMEDIA_TYPE_VIDEO:
                    decoder_type = media_base::DecoderType::DecoderTypeVideo;
                    break;
                case AVMEDIA_TYPE_AUDIO:
                    decoder_type = media_base::DecoderType::DecoderTypeAudio;
                    break;
                case AVMEDIA_TYPE_SUBTITLE:
                    decoder_type = media_base::DecoderType::DecoderTypeSubtitle;
                    break;
                default:
                    break;
            }
            if (decoder_type == media_base::DecoderType::DecoderTypeNone) {
                continue;
            }

            FFmpegCodecInfo *codec_info = new FFmpegCodecInfo();
            codec_info->type = decoder_type;
            codec_info->codec_id = FFMpegCodecIDToMeidaCodecID(av_codec->id);
            codec_info->name = av_codec->name;
            codec_info->score = 100;

            _codec_infos.push_back(codec_info);
        }
    } while (av_codec != NULL);
}

media_base::MediaDecoder *FFmpegDecoderEnumerator::create_decoder_by_codec_id(
    media_base::CodecID codec_id) {
    for (auto *codec_info : _codec_infos) {
        if (codec_info->codec_id == codec_id) {
            media_base::MediaDecoder *decoder = new FFmpegDecoder(codec_info);
            return decoder;
        }
    }
    return NULL;
}

}  // namespace media_decoder