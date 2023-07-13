#!/bin/sh

install_path="$(pwd)/../../export/ffdecoder/macos"

prefix_path="$(builtin cd $install_path; pwd)"

../../ffmpeg/configure --prefix="${prefix_path}" --disable-debug --disable-programs \
	--disable-doc --enable-pic --disable-encoders --disable-muxers --disable-demuxers --disable-videotoolbox --disable-audiotoolbox \
	--disable-protocols --disable-devices --disable-filters --disable-bsfs \
	--disable-ffplay --disable-ffprobe --disable-ffmpeg --disable-iconv
