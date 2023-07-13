#pragma once

#include "media_base/media_definition.h"

extern "C" {
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
}

namespace demuxer {

extern const AVRational gGloabalTimebase;

/**
 *  Convert ffmpeg codec id to media codec id
 *
 *  @param ffCodecID ffmpeg codec id
 *
 *  @return media codec id
 */
media_base::CodecID FFMpegCodecIDToMeidaCodecID(enum AVCodecID ff_codec_id);

/**
 *  Convert ffmpeg pixel format to media pixel format
 *
 *  @param ffPixelFormat ffmpeg pixel format
 *
 *  @return media pixel format
 */
media_base::PixelFormat FFMpegPixelFormatToMediaPixelFormat(enum AVPixelFormat ff_pixel_format);

/**
 *  Convert ffmpeg sample format to media sample format
 *
 *  @param sample_fmt ffmpeg sample format
 *
 *  @return media sample format
 */
media_base::SampleFormat FFMpegSampleFormatToMediaSampleFormat(
    enum AVSampleFormat ff_sample_format);

}  // namespace demuxer
