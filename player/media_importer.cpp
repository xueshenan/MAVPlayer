#include "media_importer.h"

#include "base/log.h"

const int kDefaultCompressedFrameBufferSize = 120;

bool MediaImporter::open(const std::string &file_path) {
    if (!_demuxer->open(file_path)) {
        return false;
    }
    unsigned int hardware_core = std::thread::hardware_concurrency();
    base::LogInfo() << "cpu core count : " << hardware_core;
    base::LogInfo() << "open demux success, file path:" << file_path;

    media_base::MovieInfo *movie_info = _demuxer->movie_info();

    media_base::StreamInfo *video_stream_info = nullptr;
    for (media_base::StreamInfo *stream_info : movie_info->streams) {
        if (stream_info->type == media_base::StreamType::StreamTypeVideo) {
            video_stream_info = stream_info;
        }
    }
    if (video_stream_info == nullptr) {
        base::LogError() << "Not found video stream";
        return false;
    }

    _decoder = decoder_enumerator.create_decoder_by_codec_id(media_base::CodecID::CodecIDH264);
    if (_decoder != nullptr) {
        media_base::AVCodecParam codec_param;
        codec_param.width = video_stream_info->width;
        codec_param.height = video_stream_info->height;
        codec_param.codec_tag = video_stream_info->codec_tag;
        codec_param.bits_per_coded_sample = video_stream_info->bits_per_coded_sample;
        codec_param.num_threads = 2;
        codec_param.extra_data = video_stream_info->extra_data;
        codec_param.extra_data_isze = video_stream_info->extra_data_size;

        bool ret = _decoder->open_codec(&codec_param);
        if (!ret) {
            return false;
        }
    }
    return true;
}

media_base::RawVideoFrame *MediaImporter::read_frame() {
    if (!_need_refresh) {
        return nullptr;
    }
    std::lock_guard<std::mutex> lock(_last_video_frame_mutex);
    if (_last_video_frame == nullptr) {
        return nullptr;
    }
    media_base::RawVideoFrame *copy_frame = new media_base::RawVideoFrame(*_last_video_frame);
    return copy_frame;
}

void MediaImporter::start_working() {
    stop_working();

    _should_exit_decoder = false;
    _decoder_thread = new std::thread(&MediaImporter::decode_compressed_frame, this);

    _should_exit_demuxer = false;
    _demuxer_thread = new std::thread(&MediaImporter::read_compressed_frame, this);
}

void MediaImporter::stop_working() {
    _should_exit_decoder = true;
    if (_decoder_thread != nullptr) {
        _decoder_thread->join();
        delete _decoder_thread;
        _decoder_thread = nullptr;
    }

    _should_exit_demuxer = true;
    if (_demuxer_thread != nullptr) {
        _demuxer_thread->join();
        delete _demuxer_thread;
        _demuxer_thread = nullptr;
    }

    {
        media_base::CompressedFrame *compressed_frame = nullptr;
        while (_compressed_frame_queue.try_dequeue(compressed_frame)) {
            delete compressed_frame;
        }
    }
}

void MediaImporter::read_compressed_frame() {
    while (!_should_exit_demuxer) {
        if (_compressed_frame_queue.size_approx() > (size_t)_max_compressed_frame_queue_size) {
            base::LogWarn() << "Too many compressed frame need consume";
            {  // clear all package
                media_base::CompressedFrame *compressed_frame = nullptr;
                while (_compressed_frame_queue.try_dequeue(compressed_frame)) {
                    delete compressed_frame;
                }
            }
            continue;
        }

        media_base::CompressedFrame *compressed_frame = _demuxer->read_frame();
        if (compressed_frame == nullptr) {
            //std::this_thread::sleep_for(std::chrono::microseconds(100));
            continue;
        }

        { _compressed_frame_queue.try_enqueue(compressed_frame); }
        // std::this_thread::sleep_for(std::chrono::microseconds(5000));
        //std::this_thread::sleep_for(std::chrono::microseconds(100));
    }
}

// static bool first_decoder = true;
// static auto now = std::chrono::system_clock::now();
// static bool first_frame = true;

static int in_frame_count = 0;
static int out_frame_count = 0;
void MediaImporter::decode_compressed_frame() {
    while (!_should_exit_decoder) {
        media_base::CompressedFrame *compressed_frame = nullptr;
        {
            if (!_compressed_frame_queue.try_dequeue(compressed_frame)) {
                continue;
            }
        }

        media_base::RawVideoFrame *video_frame = nullptr;
        // if (first_decoder) {
        //     now = std::chrono::system_clock::now();
        //     first_decoder = false;
        // }
        if (compressed_frame != nullptr) {
            in_frame_count++;
        }
        video_frame = _decoder->decode_video_frame(compressed_frame);
        if (compressed_frame != nullptr) {
            delete compressed_frame;
        }

        if (video_frame != nullptr) {
            out_frame_count++;
            // std::cout << "in frame : " << in_frame_count << " out frame : " << out_frame_count
            //           << " diff : " << in_frame_count - out_frame_count << std::endl;
            // if (first_frame) {
            //     auto current = std::chrono::system_clock::now();
            //     base::LogError()
            //         << "time duration "
            //         << std::chrono::duration_cast<std::chrono::microseconds>(current - now).count();
            //     first_frame = false;
            // }
            //replace last video frame
            std::lock_guard<std::mutex> lock(_last_video_frame_mutex);
            if (_last_video_frame != nullptr) {
                delete _last_video_frame;
                _last_video_frame = nullptr;
            }
            _last_video_frame = video_frame;
            _need_refresh = true;
        }
    }
}

MediaImporter::MediaImporter()
    : _max_compressed_frame_queue_size(kDefaultCompressedFrameBufferSize) {
    decoder_enumerator.init();
    _demuxer = media_base::CreateFFmpegDemuxer();
}

MediaImporter::~MediaImporter() {
    stop_working();
}
