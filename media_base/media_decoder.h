#pragma once

#include "media_base/compressed_frame.h"
#include "media_base/media_definition.h"
#include "media_base/raw_video_frame.h"

namespace media_base {

enum class DecoderType {
    DecoderTypeNone,
    DecoderTypeVideo,
    DecoderTypeAudio,
    DecoderTypeSubtitle,
};

struct AVCodecParam {
    int32_t num_threads;  // decoder thread count
    int32_t width;
    int32_t height;
    uint8_t *extra_data;
    int32_t extra_data_isze;
    /**
     * fourcc (LSB first, so "ABCD" -> ('D'<<24) + ('C'<<16) + ('B'<<8) + 'A').
     * This is used to work around some encoder bugs.
     * A demuxer should set this to what is stored in the field used to identify the codec.
     * If there are multiple such fields in a container then the demuxer should choose the one
     * which maximizes the information about the used codec.
     * If the codec tag field in a container is larger than 32 bits then the demuxer should
     * remap the longer ID to 32 bits with a table or other structure. Alternatively a new
     * extra_codec_tag + size could be added but for this a clear advantage must be demonstrated
     * first.
     * - encoding: Set by user, if not then the default based on codec_id will be used.
     * - decoding: Set by user, will be converted to uppercase by libavcodec during init.
     */
    uint32_t codec_tag;
    /**
     * bits per sample/pixel from the demuxer (needed for huffyuv).
     * - encoding: Set by libavcodec.
     * - decoding: Set by user.
     */
    int32_t bits_per_coded_sample;

    media_base::SampleFormat sample_format;
    int32_t channels;
    int32_t samplerate;
    int32_t frame_size;
};

/**
 * media decoder
 */
class MediaDecoder {
public:
    virtual bool open_codec(AVCodecParam *codec_param) = 0;

    virtual RawVideoFrame *decode_video_frame(CompressedFrame *compressed_frame) = 0;
};

}  // namespace media_base