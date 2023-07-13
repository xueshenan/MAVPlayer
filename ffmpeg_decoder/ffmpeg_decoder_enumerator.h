#pragma once

#include <string>
#include <vector>

#include "media_base/media_decoder.h"
#include "media_base/media_definition.h"

namespace media_decoder {

struct FFmpegCodecInfo {
    media_base::DecoderType type;
    media_base::CodecID codec_id;
    std::string name;
    int32_t score;
};

/**
 *  Enum all ffmpeg decoders
 */
class FFmpegDecoderEnumerator final {
public:
    /**
     *  init all support decoder
     */
    void init();

    /**
     *  Create ffmpeg decoder by codec id
     *
     *  @param codec_id codec id
     *
     *  @return media_base::MediaDecoder on success
     */
    media_base::MediaDecoder *create_decoder_by_codec_id(media_base::CodecID codec_id);
public:
    FFmpegDecoderEnumerator();
    ~FFmpegDecoderEnumerator();
    FFmpegDecoderEnumerator(const FFmpegDecoderEnumerator &decoderEnumerator) = delete;
    FFmpegDecoderEnumerator &operator=(const FFmpegDecoderEnumerator &decoderEnumerator) = delete;
private:
    std::vector<FFmpegCodecInfo *> _codec_infos;
};

}  // namespace media_decoder