project(player)

set(PLAYER_NAME "MAVPlayer")

aux_source_directory("." PLAYER_SRC)

add_executable(${PLAYER_NAME} ${PLAYER_SRC})

target_include_directories(${PLAYER_NAME}
    PRIVATE
    ${CMAKE_SOURCE_DIR})

find_package(PkgConfig REQUIRED)

pkg_check_modules(SDL2 sdl2 REQUIRED IMPORTED_TARGET)

target_link_libraries(${PLAYER_NAME} PRIVATE PkgConfig::SDL2)

target_link_libraries(${PLAYER_NAME}
    PRIVATE
    base
    ffdemuxer
    ffdecoder)