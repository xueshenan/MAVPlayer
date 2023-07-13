#pragma once

#include "media_base/media_definition.h"

namespace media_base {

struct CompressedFrame {
    StreamType type;
    CodecID codec_id;
    bool key_frame;           ///<  key frame flag
    int64_t dts;              ///<  dts value
    int64_t pts;              ///<  pts value
    int64_t duration;         ///<  frame duration
    int64_t position;         ///<  frame position
    int8_t *frame_data;       ///<  frame  data in memory
    int32_t frame_data_size;  ///< frame data size
public:
    CompressedFrame();
    CompressedFrame(const CompressedFrame &);
    CompressedFrame &operator=(const CompressedFrame &compressed_frame);
    ~CompressedFrame();
private:
    void CopyInfo(const CompressedFrame &compressed_frame);
};

}  // namespace media_base