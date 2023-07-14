#!/bin/sh

install_path="$(pwd)/../../export/ffmpeg/macos"

mkdir -p ${install_path}

prefix_path="$(builtin cd $install_path; pwd)"

../../ffmpeg/configure --prefix="${prefix_path}" --disable-debug --disable-programs \
    --disable-doc --enable-pic --disable-encoders --disable-muxers --disable-videotoolbox --disable-audiotoolbox \
    --disable-decoders --enable-decoder=h264 --enable-decoder=hevc --enable_decoder=aac --enable_decoder=ac3 \
    --enable-protocols --disable-devices --disable-filters --enable-bsfs \
    --disable-ffplay --disable-ffprobe --disable-ffmpeg \
    --disable-iconv --disable-openssl --disable-securetransport