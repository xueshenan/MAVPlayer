#include "ffmpeg_common.h"

namespace media_decoder {

media_base::CodecID FFMpegCodecIDToMeidaCodecID(enum AVCodecID ff_codec_id) {
    media_base::CodecID codec_id = media_base::CodecID::CodecIDNone;
    switch (ff_codec_id) {
        case AV_CODEC_ID_H264:
            codec_id = media_base::CodecID::CodecIDH264;
            break;
        case AV_CODEC_ID_HEVC:
            codec_id = media_base::CodecID::CodecIDHEVC;
            break;
        case AV_CODEC_ID_AAC:
            codec_id = media_base::CodecID::CodecIDAAC;
            break;
        case AV_CODEC_ID_AC3:
            codec_id = media_base::CodecID::CodecIDAC3;
            break;
        default:
            break;
    }
    return codec_id;
}

enum AVCodecID MediaCodecIDToFFMpegCodecID(media_base::CodecID codec_id) {
    enum AVCodecID ff_codec_id = AV_CODEC_ID_NONE;
    switch (codec_id) {
        case media_base::CodecID::CodecIDH264:
            ff_codec_id = AV_CODEC_ID_H264;
            break;
        case media_base::CodecID::CodecIDHEVC:
            ff_codec_id = AV_CODEC_ID_HEVC;
            break;
        case media_base::CodecID::CodecIDAAC:
            ff_codec_id = AV_CODEC_ID_AAC;
            break;
        case media_base::CodecID::CodecIDAC3:
            ff_codec_id = AV_CODEC_ID_AC3;
            break;
        default:
            break;
    }
    return ff_codec_id;
}

media_base::PixelFormat FFMpegPixelFormatToMediaPixelFormat(enum AVPixelFormat ff_pixel_format) {
    media_base::PixelFormat pixel_format = media_base::PixelFormat::PixelFormatNone;
    switch (ff_pixel_format) {
        case AV_PIX_FMT_YUV420P:
            pixel_format = media_base::PixelFormat::PixelFormatYUV420P;
            break;
        case AV_PIX_FMT_YUV422P:
            pixel_format = media_base::PixelFormat::PixelFormatYUV422P;
            break;
        case AV_PIX_FMT_YUV444P:
            pixel_format = media_base::PixelFormat::PixelFormatYUV444P;
            break;
        case AV_PIX_FMT_NV12:
            pixel_format = media_base::PixelFormat::PixelFormatNV12;
            break;
        case AV_PIX_FMT_RGB24:
            pixel_format = media_base::PixelFormat::PixelFormatRGB24;
            break;
        case AV_PIX_FMT_BGR24:
            pixel_format = media_base::PixelFormat::PixelFormatBGR24;
            break;
        default:
            break;
    }
    return pixel_format;
}

}  // namespace media_decoder