#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

#include "media_base/stream_info.h"

namespace media_base {

/**
 * MovieInfo
 */
struct MovieInfo final {
    int32_t id;          // movie id -1 for unknown
    std::string name;    // movie name
    std::string format;  // movie format e.g. mov mp4 mxf mts
    std::vector<StreamInfo *> streams;
    std::unordered_map<std::string, std::string> metadata;
public:
    MovieInfo();
    MovieInfo(const MovieInfo &movie_info);
    MovieInfo &operator=(const MovieInfo &movie_info);
    ~MovieInfo();
private:
    void CopyInfo(const MovieInfo &movieInfo);
};

}  // namespace media_base