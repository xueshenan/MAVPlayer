#!/bin/sh

../../ffmpeg/configure --prefix="../../export/ffdemuxer/macos/" --disable-debug \
    --disable-doc --enable-pic --disable-encoders --disable-muxers --disable-decoders --enable-decoder=aac --enable-decoder=ac3 \
    --enable-network --enable-protocols --disable-devices --disable-filters --enable-bsfs --disable-iconv \
    --disable-programs
