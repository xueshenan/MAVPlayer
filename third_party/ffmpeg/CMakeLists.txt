cmake_minimum_required(VERSION 3.14)
cmake_policy(VERSION 3.14)
if(POLICY CMP0135) # DOWNLOAD_EXTRACT_TIMESTAMP
  cmake_policy(SET CMP0135 OLD)
endif()

project(external-ffmpeg)

set(FFMPEG_SOURCE_DIR ${PROJECT_BINARY_DIR}/ffmpeg-src CACHE PATH "Install path of FFmpeg source code")

include(FetchContent)

FetchContent_Declare(
  ffmpeg
  URL https://ffmpeg.org/releases/ffmpeg-6.0.tar.xz
  URL_HASH MD5=47b6c5d930937413c3e308e2fdb3dfb5
  SOURCE_DIR ${FFMPEG_SOURCE_DIR}
)

if (EXISTS ${FFMPEG_SOURCE_DIR}/configure)
  set(FETCHCONTENT_SOURCE_DIR_FFMPEG ${FFMPEG_SOURCE_DIR})
endif()

FetchContent_GetProperties(ffmpeg)
if (NOT ffmpeg_POPULATED)
  FetchContent_Populate(ffmpeg)
endif()

# Cross compiling configurations
if (CMAKE_CROSSCOMPILING)
  if (CMAKE_SYSTEM_PROCESSOR MATCHES arm)
    list(APPEND FFMPEG_CONFIGURE_ARGS
      "--arch=arm"
      "--enable-neon"
      "--enable-thumb"
    )
  endif()
  if (CMAKE_SYSTEM_PROCESSOR MATCHES aarch64 OR CMAKE_SYSTEM_PROCESSOR MATCHES arm64)
    list(APPEND FFMPEG_CONFIGURE_ARGS
      "--arch=aarch64"
      "--enable-thumb"
    )
  endif()
  list(APPEND FFMPEG_CONFIGURE_ARGS
    "--enable-cross-compile"
    "--target-os=linux"
    "--enable-pic"
    "--enable-optimizations"
    "--enable-debug"
    "--enable-small"
    "--cc=${CMAKE_C_COMPILER}"
    "--extra-cflags=${CMAKE_C_FLAGS}"
  )
endif()

# Disable everything not needed
list(APPEND FFMPEG_CONFIGURE_ARGS
  "--prefix=${CMAKE_INSTALL_PREFIX}"
  "--disable-gpl"
  "--disable-nonfree"
  "--enable-runtime-cpudetect"
  "--disable-gray"
  "--disable-swscale-alpha"
  "--disable-programs"
  "--disable-doc"
  "--disable-htmlpages"
  "--disable-manpages"
  "--disable-podpages"
  "--disable-txtpages"
)

# Component options
list(APPEND FFMPEG_CONFIGURE_ARGS
  "--disable-avdevice"
  "--enable-avcodec"
  "--enable-avformat"
  "--enable-avutil"
  "--enable-swresample"
  "--disable-postproc"
  "--enable-avfilter"
  "--enable-network"
)

# Hardware acceleration options
list(APPEND FFMPEG_CONFIGURE_ARGS
  "--disable-hwaccels"
  "--disable-d3d11va"
  "--disable-dxva2"
  "--disable-vaapi"
  "--disable-vdpau"
  "--disable-videotoolbox"
)

# ./configure --list-decoders
list(APPEND FFMPEG_CONFIGURE_ARGS
  "--disable-decoders"
  "--enable-decoder=h264"
  "--enable-decoder=hevc"
  "--enable-decoder=vp8"
  "--enable-decoder=vp9"
)

# ./configure --list-encoders
list(APPEND FFMPEG_CONFIGURE_ARGS
  "--disable-encoders"
)

# ./configure --list-demuxers
list(APPEND FFMPEG_CONFIGURE_ARGS
  "--disable-demuxers"
  "--enable-demuxer=aac"
  "--enable-demuxer=concat"
  "--enable-demuxer=data"
  "--enable-demuxer=flv"
  "--enable-demuxer=hls"
  "--enable-demuxer=live_flv"
  "--enable-demuxer=mov"
  "--enable-demuxer=mp3"
  "--enable-demuxer=mpegps"
  "--enable-demuxer=mpegts"
  "--enable-demuxer=mpegvideo"
  "--enable-demuxer=flac"
  "--enable-demuxer=hevc"
  "--enable-demuxer=rtp"
  "--enable-demuxer=rtsp"
  "--enable-demuxer=rawvideo"
  "--enable-demuxer=yuv4mpegpipe"
)

# ./configure --list-muxers
list(APPEND FFMPEG_CONFIGURE_ARGS
  "--disable-muxers"
)

# ./configure --list-parsers
list(APPEND FFMPEG_CONFIGURE_ARGS
  "--disable-parsers"
  "--enable-parser=aac"
  "--enable-parser=aac_latm"
  "--enable-parser=h264"
  "--enable-parser=flac"
  "--enable-parser=hevc"
)

# ./configure --list-bsf
list(APPEND FFMPEG_CONFIGURE_ARGS
  "--enable-bsfs"
  "--disable-bsf=chomp"
  "--disable-bsf=dca_core"
  "--disable-bsf=dump_extradata"
  "--disable-bsf=hevc_mp4toannexb"
  "--disable-bsf=imx_dump_header"
  "--disable-bsf=mjpeg2jpeg"
  "--disable-bsf=mjpega_dump_header"
  "--disable-bsf=mov2textsub"
  "--disable-bsf=mp3_header_decompress"
  "--disable-bsf=mpeg4_unpack_bframes"
  "--disable-bsf=noise"
  "--disable-bsf=remove_extradata"
  "--disable-bsf=text2movsub"
  "--disable-bsf=vp9_superframe"
  "--disable-bsf=eac3_core"
)

# ./configure --list-protocols
list(APPEND FFMPEG_CONFIGURE_ARGS
  "--enable-protocols"
  "--enable-protocol=async"
  "--disable-protocol=bluray"
  "--disable-protocol=concat"
  "--disable-protocol=crypto"
  "--disable-protocol=ffrtmpcrypt"
  "--enable-protocol=ffrtmphttp"
  "--disable-protocol=gopher"
  "--disable-protocol=icecast"
  "--disable-protocol=librtmp*"
  "--disable-protocol=libssh"
  "--disable-protocol=md5"
  "--disable-protocol=mmsh"
  "--disable-protocol=mmst"
  "--disable-protocol=rtmp*"
  "--enable-protocol=rtmp"
  "--enable-protocol=rtmpt"
  "--enable-protocol=rtp"
  "--enable-protocol=udp"
  "--disable-protocol=sctp"
  "--disable-protocol=srtp"
  "--disable-protocol=subfile"
  "--disable-protocol=unix"
)

#
list(APPEND FFMPEG_CONFIGURE_ARGS
  "--disable-devices"
  "--disable-filters"
)

# External library support
list(APPEND FFMPEG_CONFIGURE_ARGS
  "--disable-iconv"
  "--disable-audiotoolbox"
  "--disable-videotoolbox"
  "--disable-linux-perf"
  "--disable-bzlib"
  "--disable-openssl"
  "--disable-securetransport"
)

message(STATUS "Preparing external project \"ffmpeg\" with args:")
foreach(CMAKE_ARG ${CMAKE_ARGS})
  message(STATUS "-- ${CMAKE_ARG}")
endforeach()

message(STATUS "SOURCE DIR: ${FFMPEG_SOURCE_DIR}")

include(ExternalProject)

ExternalProject_Add(
  ffmpeg
  SOURCE_DIR "${FFMPEG_SOURCE_DIR}"
  CONFIGURE_COMMAND <SOURCE_DIR>/configure ${FFMPEG_CONFIGURE_ARGS}
  BUILD_COMMAND make V=1
  INSTALL_COMMAND make install
  PREFIX ffmpeg
  CMAKE_ARGS "${CMAKE_ARGS}"
)