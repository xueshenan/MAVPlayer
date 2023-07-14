#pragma once

#include <atomic>
#include <chrono>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>

#include "ffmpeg_decoder/ffmpeg_decoder_enumerator.h"
#include "media_base/media_decoder.h"
#include "media_base/media_demuxer.h"
#include "media_base/raw_video_frame.h"

class MediaImporter {
public:
    bool open(const std::string &file_path);
    media_base::RawVideoFrame *read_frame();
    void start_working();
    void stop_working();
public:
    MediaImporter();
    ~MediaImporter();
private:  // backend work thread
    void read_compressed_frame();
    std::thread *_demuxer_thread{nullptr};
    std::atomic<bool> _should_exit_demuxer{false};
    void decode_compressed_frame();
    std::thread *_decoder_thread{nullptr};
    std::atomic<bool> _should_exit_decoder{false};
private:
    const int32_t _max_compressed_frame_queue_size;
    std::mutex _frame_queue_mutex{};
    std::atomic<int32_t> _compressed_frame_queue_size;
    std::queue<media_base::CompressedFrame *> _compressed_frame_queue;

    mutable std::mutex _last_video_frame_mutex{};
    mutable media_base::RawVideoFrame *_last_video_frame;
private:
    media_base::MediaDemuxer *_demuxer;
    media_base::MediaDecoder *_decoder;
    media_decoder::FFmpegDecoderEnumerator decoder_enumerator;
};