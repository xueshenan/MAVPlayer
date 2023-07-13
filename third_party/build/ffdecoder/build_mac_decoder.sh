#!/bin/sh

../../ffmpeg/configure --prefix="../../export/ffdecoder/macos" --disable-debug --disable-programs \
	--disable-doc --enable-pic --disable-encoders --disable-muxers --disable-demuxers --disable-videotoolbox --disable-audiotoolbox \
	--disable-protocols --disable-devices --disable-filters --disable-bsfs \
	--disable-ffplay --disable-ffprobe --disable-ffmpeg --disable-iconv
