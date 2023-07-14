cmake_minimum_required(VERSION 3.13)

include(cmake/color_message.cmake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra -pedantic")

set(FFMPEG_EXPORT_PLATFORM_NAME "")
if(CMAKE_HOST_APPLE)
    message(STATUS "build macos platform")
endif()

project(MAVPlayer)

set(DEPS_BUILD_PATH "${PROJECT_BINARY_DIR}/third_party" CACHE PATH "build path for the dependencies.")
set(DEPS_INSTALL_PATH "${DEPS_BUILD_PATH}/install" CACHE PATH "Install path for the dependencies.")
set(BUILD_DEPENDENCY ON)

add_subdirectory(third_party)

add_subdirectory(base)

add_subdirectory(media_base)

add_subdirectory(ffmpeg_demuxer)

add_subdirectory(ffmpeg_decoder)

add_subdirectory(player)