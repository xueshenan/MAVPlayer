#include "media_base/movie_info.h"

namespace media_base {

MovieInfo::MovieInfo() {
    id = -1;
}

MovieInfo::MovieInfo(const MovieInfo &movieInfo) {
    CopyInfo(movieInfo);
}

MovieInfo &MovieInfo::operator=(const MovieInfo &movieInfo) {
    if (this == &movieInfo) {
        return *this;
    }
    CopyInfo(movieInfo);
    return *this;
}

MovieInfo::~MovieInfo() {
    for (uint32_t i = 0; i < streams.size(); i++) {
        StreamInfo *stream_info = streams[i];
        delete stream_info;
        streams[i] = nullptr;
    }
    streams.clear();
}

void MovieInfo::CopyInfo(const MovieInfo &movie_info) {
    id = movie_info.id;
    name = movie_info.name;
    format = movie_info.format;
    metadata.clear();
    metadata = movie_info.metadata;

    for (uint32_t i = 0; i < movie_info.streams.size(); i++) {
        StreamInfo *stream_info = new StreamInfo(*movie_info.streams[i]);
        streams.push_back(stream_info);
    }
}

}  // namespace media_base