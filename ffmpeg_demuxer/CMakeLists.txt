project(ffmpeg_demuxer)

set(FFMPEG_DEMUXER_LIBRARY_NAME "ffdemuxer")

aux_source_directory("." FFMPEG_DEMUXER_SRC)

add_library(${FFMPEG_DEMUXER_LIBRARY_NAME} STATIC ${FFMPEG_DEMUXER_SRC})

target_include_directories(${FFMPEG_DEMUXER_LIBRARY_NAME}
    PRIVATE
    ${CMAKE_SOURCE_DIR})

find_package(PkgConfig REQUIRED)

pkg_check_modules(libavcodec libavcodec REQUIRED IMPORTED_TARGET)
pkg_check_modules(libavformat libavformat REQUIRED IMPORTED_TARGET)
pkg_check_modules(libavutil libavutil REQUIRED IMPORTED_TARGET)

message(STATUS "ffmpeg demuxer include path : ${libavcodec_INCLUDE_DIRS}")

target_link_libraries(${FFMPEG_DEMUXER_LIBRARY_NAME} 
    PRIVATE
    PkgConfig::libavcodec
    PkgConfig::libavformat
    PkgConfig::libavutil)

target_link_libraries(${FFMPEG_DEMUXER_LIBRARY_NAME} PRIVATE mediabase)


