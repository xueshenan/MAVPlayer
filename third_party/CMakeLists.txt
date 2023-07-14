cmake_minimum_required(VERSION 3.14)

project(third_party)

include(cmake/build_target.cmake)

if(BUILD_DEPENDENCY)
    list(APPEND CMAKE_PREFIX_PATH "${DEPS_INSTALL_PATH}")
    set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} PARENT_SCOPE)

    build_target(ffmpeg)
endif()
