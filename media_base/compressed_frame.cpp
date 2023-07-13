#include "media_base/compressed_frame.h"

#include <string.h>

#include <cstdlib>

namespace media_base {

CompressedFrame::CompressedFrame() {
    type = StreamType::StreamTypeUnknown;
    codec_id = CodecID::CodecIDNone;
    dts = -1;
    pts = -1;
    duration = -1;
    position = -1;
    frame_data = NULL;
    frame_data_size = 0;
}

CompressedFrame::CompressedFrame(const CompressedFrame &compressed_frame) {
    CopyInfo(compressed_frame);
}

CompressedFrame &CompressedFrame::operator=(const CompressedFrame &compressed_frame) {
    if (this == &compressed_frame) {
        return *this;
    }
    CopyInfo(compressed_frame);
    return *this;
}

CompressedFrame::~CompressedFrame() {
    if (frame_data_size > 0 && frame_data != NULL) {
        free(frame_data);
        frame_data = NULL;
        frame_data_size = 0;
    }
}

void CompressedFrame::CopyInfo(const CompressedFrame &compressed_frame) {
    type = compressed_frame.type;
    codec_id = compressed_frame.codec_id;
    key_frame = compressed_frame.key_frame;
    dts = compressed_frame.dts;
    pts = compressed_frame.pts;
    duration = compressed_frame.duration;
    position = compressed_frame.duration;
    if (frame_data_size > 0 && frame_data != NULL) {
        free(frame_data);
        frame_data = NULL;
        frame_data_size = 0;
    }

    if (compressed_frame.frame_data_size > 0 && compressed_frame.frame_data != NULL) {
        frame_data_size = compressed_frame.frame_data_size;
        frame_data = (int8_t *)malloc(frame_data_size);
        memcpy(frame_data, compressed_frame.frame_data, frame_data_size);
    }
}

}  // namespace media_base