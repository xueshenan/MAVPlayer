#pragma once

#include "media_base/movie_info.h"

namespace media_base {

struct CompressedFrame;

/**
 *  Demuxer media file
 *  This is only interface object,all method are implement by subclass
 *  call 'CreateXXXDemuxer' function to create instance.
 */
class MediaDemuxer {
public:
    /**
     *  open media demuxer
     *
     *  @param file_path input file path, support rtsp,rtmp stream url
     *
     *  @return success or failed
     */
    virtual bool open(const std::string &file_path) = 0;

    virtual void close() = 0;

    virtual MovieInfo *movie_info() = 0;

    virtual CompressedFrame *read_frame() = 0;

    /*
    *  @return current timestamp in microsecond
    */
    virtual int64_t current_timestamp() = 0;
    /**
     *  whether read end of the file
     *
     *  @return true or false
     */
    virtual bool eof() = 0;
};

}  // namespace media_base