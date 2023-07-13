#!/bin/sh

install_path="$(pwd)/../../export/ffdemuxer/macos"

prefix_path="$(builtin cd $install_path; pwd)"

../../ffmpeg/configure --prefix="${prefix_path}" --disable-debug \
    --disable-doc --enable-pic --disable-encoders --disable-muxers --disable-decoders --enable-decoder=aac --enable-decoder=ac3 \
    --enable-network --enable-protocols --disable-devices --disable-filters --enable-bsfs --disable-iconv \
    --disable-programs
