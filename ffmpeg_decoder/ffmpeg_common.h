#pragma once

#include "media_base/media_definition.h"

extern "C" {
#include "libavcodec/avcodec.h"
}

namespace media_decoder {

/**
 *  Convert ffmpeg codec id to media codec id
 *
 *  @param ffCodecID ffmpeg codec id
 *
 *  @return media codec id
 */
media_base::CodecID FFMpegCodecIDToMeidaCodecID(enum AVCodecID ff_codec_id);

enum AVCodecID MediaCodecIDToFFMpegCodecID(media_base::CodecID codec_id);

/**
 *  Convert ffmpeg pixel format to media_base pixel format
 *
 *  @param ffPixelFormat ffmpeg pixel format
 *
 *  @return media_base pixel format
 */
media_base::PixelFormat FFMpegPixelFormatToMediaPixelFormat(enum AVPixelFormat ffPixelFormat);

}  // namespace media_decoder