#pragma once

#include "media_base/media_decoder.h"
#include "media_base/media_demuxer.h"
#include "media_base/raw_video_frame.h"

class MediaImporter {
public:
    bool open(const std::string &file_path);
    media_base::RawVideoFrame *ReadFrame();
public:
    MediaImporter();
    ~MediaImporter();
private:
    media_base::MediaDemuxer *_demuxer;
    media_base::MediaDecoder *_decoder;
}