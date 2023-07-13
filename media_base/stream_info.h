#pragma once

#include <string>
#include <unordered_map>

#include "media_base/media_definition.h"

namespace media_base {

/**
 * media stream info
 */
struct StreamInfo final {
    int32_t id;  ///< stream id
    StreamType type;
    std::string name;
    CodecID codec_id;
    std::string codec_name;
    std::string language;
    int64_t bitrate;     ///< The average bitrate of the encoded data (in bits per second)
    int64_t duration;    ///< stream duration in ms, -1 is unkonwn
    uint32_t codec_tag;  ///< four byte tag
    int32_t bits_per_coded_sample;

    ///< video param
    int32_t width;
    int32_t height;
    PixelFormat pixel_format;
    Rational framerate;
    Rational pixel_aspect;

    ///< audio param
    SampleFormat sample_format;
    int32_t samplerate;
    int32_t channels;
    int32_t frame_size;  ///< Compassde audio frame size

    std::unordered_map<std::string, std::string> metadata;  // stream meta data map
    uint8_t *extra_data;                                    // stream extra data
    int32_t extra_data_size;                                // stream extra data size
public:
    StreamInfo();
    StreamInfo(const StreamInfo &stream_info);
    StreamInfo &operator=(const StreamInfo &stream_info);
    ~StreamInfo();
private:
    void CopyStreamInfo(const StreamInfo &stream_info);
};

}  // namespace media_base