#include "media_base/stream_info.h"

#include <cstdlib>

namespace media_base {

StreamInfo::StreamInfo() {
    id = -1;
    type = StreamType::StreamTypeUnknown;
    bitrate = -1;
    duration = -1;

    width = -1;
    height = -1;
    framerate.numerator = 1;
    framerate.denominator = 1;
    pixel_aspect.numerator = 1;
    pixel_aspect.denominator = 1;

    sample_format = SampleFormat::SampleFormatNone;
    samplerate = -1;
    channels = -1;

    frame_size = 0;

    extra_data = NULL;
    extra_data_size = 0;
}

StreamInfo::StreamInfo(const StreamInfo &stream_info) {
    CopyStreamInfo(stream_info);
}

StreamInfo &StreamInfo::operator=(const StreamInfo &stream_info) {
    if (this == &stream_info) {
        return *this;
    }
    CopyStreamInfo(stream_info);
    return *this;
}

StreamInfo::~StreamInfo() {}

void StreamInfo::CopyStreamInfo(const StreamInfo &stream_info) {
    id = stream_info.id;
    type = stream_info.type;
    name = stream_info.name;
    codec_id = stream_info.codec_id;
    codec_name = stream_info.codec_name;
    language = stream_info.language;
    bitrate = stream_info.bitrate;
    duration = stream_info.duration;

    codec_tag = stream_info.codec_tag;
    bits_per_coded_sample = stream_info.bits_per_coded_sample;

    width = stream_info.width;
    height = stream_info.height;
    pixel_format = stream_info.pixel_format;
    framerate = stream_info.framerate;
    pixel_aspect = stream_info.pixel_aspect;

    sample_format = stream_info.sample_format;
    samplerate = stream_info.samplerate;
    channels = stream_info.channels;
    frame_size = stream_info.frame_size;

    metadata.clear();
    metadata = stream_info.metadata;

    if (extra_data_size > 0 && extra_data != NULL) {
        free(extra_data);
    }

    if (stream_info.extra_data_size > 0 && stream_info.extra_data != NULL) {
        extra_data_size = stream_info.extra_data_size;
        extra_data = (uint8_t *)malloc(extra_data_size);
        memcpy(extra_data, stream_info.extra_data, extra_data_size);
    }
}

}  // namespace media_base