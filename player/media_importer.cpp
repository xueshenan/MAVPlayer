#include "media_importer.h"

#include "ffmpeg_demuxer/ffmpeg_demuxer.h"

bool MediaImporter::open(const std::string &file_path) {
    return true;
}

MediaImporter::MediaImporter() {
    _demuxer = media_demuxer::CreateFFMpegDemuxer();
}

MediaImporter::~MediaImporter() {
    if (_demuxer != nullptr) {
        delete _demuxer;
    }
}