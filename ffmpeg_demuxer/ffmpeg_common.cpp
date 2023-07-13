#include "ffmpeg_common.h"

namespace demuxer {

const AVRational gGloabalTimeBase = {1, AV_TIME_BASE};

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

media_base::SampleFormat FFMpegSampleFormatToMediaSampleFormat(
    enum AVSampleFormat ff_sample_format) {
    media_base::SampleFormat sample_format = media_base::SampleFormat::SampleFormatNone;
    switch (ff_sample_format) {
        case AV_SAMPLE_FMT_U8:
            sample_format = media_base::SampleFormat::SampleFormatU8;
            break;
        case AV_SAMPLE_FMT_U8P:
            sample_format = media_base::SampleFormat::SampleFormatU8P;
            break;
        case AV_SAMPLE_FMT_S16:
            sample_format = media_base::SampleFormat::SampleFormatS16;
            break;
        case AV_SAMPLE_FMT_S16P:
            sample_format = media_base::SampleFormat::SampleFormatS16P;
            break;
        case AV_SAMPLE_FMT_S32:
            sample_format = media_base::SampleFormat::SampleFormatS32;
            break;
        case AV_SAMPLE_FMT_S32P:
            sample_format = media_base::SampleFormat::SampleFormatS32P;
            break;
        case AV_SAMPLE_FMT_FLT:
            sample_format = media_base::SampleFormat::SampleFormatFLT;
            break;
        case AV_SAMPLE_FMT_FLTP:
            sample_format = media_base::SampleFormat::SampleFormatFLTP;
            break;
        case AV_SAMPLE_FMT_DBL:
            sample_format = media_base::SampleFormat::SampleFormatDBL;
            break;
        case AV_SAMPLE_FMT_DBLP:
            sample_format = media_base::SampleFormat::SampleFormatDBLP;
            break;
        default:
            break;
    }
    return sample_format;
}

}  // namespace demuxer