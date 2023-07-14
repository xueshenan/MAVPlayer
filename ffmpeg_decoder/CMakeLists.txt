project(ffmpeg_decoder)

set(FFMPEG_DECODER_LIBRARY_NAME "ffdecoder")

aux_source_directory("." FFMPEG_DECODER_SRC)

add_library(${FFMPEG_DECODER_LIBRARY_NAME} STATIC ${FFMPEG_DECODER_SRC})

target_include_directories(${FFMPEG_DECODER_LIBRARY_NAME}
    PRIVATE
    ${CMAKE_SOURCE_DIR})

find_package(PkgConfig REQUIRED)

pkg_check_modules(libavcodec libavcodec REQUIRED IMPORTED_TARGET)
pkg_check_modules(libavformat libavformat REQUIRED IMPORTED_TARGET)
pkg_check_modules(libavutil libavutil REQUIRED IMPORTED_TARGET)

message(STATUS "ffmpeg decoder include path : ${libavcodec_INCLUDE_DIRS}")

target_link_libraries(${FFMPEG_DECODER_LIBRARY_NAME} PRIVATE PkgConfig::libavcodec)
target_link_libraries(${FFMPEG_DECODER_LIBRARY_NAME} PRIVATE PkgConfig::libavformat)
target_link_libraries(${FFMPEG_DECODER_LIBRARY_NAME} PRIVATE PkgConfig::libavutil)