project(ffmpeg_demuxer)

set(FFMPEG_DEMUXER_LIBRARY_NAME "ffdemuxer")

aux_source_directory("." FFMPEG_DEMUXER_SRC)

add_library(${FFMPEG_DEMUXER_LIBRARY_NAME} STATIC ${FFMPEG_DEMUXER_SRC})

target_include_directories(${FFMPEG_DEMUXER_LIBRARY_NAME}
    PRIVATE
    ${CMAKE_SOURCE_DIR})

set(FFMPEG_LIBRARY_PATH "${CMAKE_SOURCE_DIR}/third_party/export/ffmpeg/${FFMPEG_EXPORT_PLATFORM_NAME}/")

list(APPEND CMAKE_PREFIX_PATH "${FFMPEG_LIBRARY_PATH}")
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} PARENT_SCOPE)

find_package(PkgConfig REQUIRED)

pkg_check_modules(libavcodec libavcodec REQUIRED IMPORTED_TARGET)
pkg_check_modules(libavformat libavformat REQUIRED IMPORTED_TARGET)
pkg_check_modules(libavutil libavutil REQUIRED IMPORTED_TARGET)

message(STATUS "ffmpeg demuxer include path : ${libavcodec_INCLUDE_DIRS}")

target_link_libraries(${FFMPEG_DEMUXER_LIBRARY_NAME} PRIVATE PkgConfig::libavcodec)
target_link_libraries(${FFMPEG_DEMUXER_LIBRARY_NAME} PRIVATE PkgConfig::libavformat)
target_link_libraries(${FFMPEG_DEMUXER_LIBRARY_NAME} PRIVATE PkgConfig::libavutil)


