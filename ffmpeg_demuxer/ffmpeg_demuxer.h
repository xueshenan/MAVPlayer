#pragma once

#include "ffmpeg_demuxer/ffmpeg_common.h"
#include "media_base/media_demuxer.h"

namespace media_demuxer {

class FFmpegDemuxer : public media_base::MediaDemuxer {
public:  // media_base::MediaDemuxer interface
    virtual bool open(const std::string &file_path) override;
    virtual void close() override;
    virtual media_base::MovieInfo *movie_info() override;
    virtual media_base::CompressedFrame *read_frame() override;
    virtual int64_t current_timestamp() override;
    virtual bool eof() override;
public:
    FFmpegDemuxer();
    virtual ~FFmpegDemuxer();
private:
    void init_ffmpeg();
    void build_movie_info(const std::string &file_path);
    bool is_valid_framerate(int32_t numerator, int32_t denominator);
private:
    AVFormatContext *_format_context;
    media_base::MovieInfo _movie_info;
    bool _eof;
    bool _got_key_frame;
};

}  // namespace media_demuxer