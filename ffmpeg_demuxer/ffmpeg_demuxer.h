#pragma once

#include "media_base/media_demuxer.h"

namespace demuxer {

class FFMpegDemuxer : public media_base::MediaDemuxer {
public:  // media_base::MediaDemuxer interface
    virtual bool open(const std::string &file_path) override;
    virtual void close() override;
    virtual MovieInfo *movie_info() override;
    virtual media_base::CompressedFrame *read_frame() override;
    virtual int64_t current_timestamp() override;
    virtual bool eof() override;
public:
    FFMpegDemuxer();
    virtual ~FFMpegDemuxer();
private:
    void init_ffmpeg();
    void build_movie_info(const std::string &filePath);
    bool is_valid_framerate(int32_t numerator, int32_t denominator);
private:
    AVFormatContext *_format_context;
    media_base::MovieInfo _movie_info;
    bool _eof;
};

}  // namespace demuxer