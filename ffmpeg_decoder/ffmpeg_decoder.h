#pragma once

#include "ffmpeg_common.h"
#include "ffmpeg_decoder_enumerator.h"
#include "media_base/media_decoder.h"

namespace media_decoder {

class FFmpegDecoder : public media_base::MediaDecoder {
public:
    virtual bool open_codec(media_base::AVCodecParam *codec_param) override;

    virtual media_base::RawVideoFrame *decode_video_frame(
        media_base::CompressedFrame *compressed_frame) override;
public:
    FFmpegDecoder(FFmpegCodecInfo *codec_info);
    virtual ~FFmpegDecoder();
private:
    FFmpegCodecInfo _codec_info;
    const AVCodec *_avcodec;
    AVCodecContext *_avcodec_context;
    uint8_t *_extra_data;
    int32_t _extra_data_size;
};

}  // namespace media_decoder