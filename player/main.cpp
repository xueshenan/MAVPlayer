#include <algorithm>
#include <bitset>
#include <cstdint>
#include <iostream>
#include <vector>

#include "SDL2/SDL.h"
#include "base/log.h"
#include "base/unused.h"
#include "media_importer.h"

const int kWindowWidth = 1280;
const int kWindowHeight = 720;

void usage(const char *bin_name);

int main(int argc, const char *argv[]) {
    std::string file_path;
    for (int i = 1; i < argc; i++) {
        const std::string current_arg = argv[i];
        if (current_arg == "-h" || current_arg == "--help") {
            usage(argv[0]);
            return 0;
        } else if (current_arg == "-f" || current_arg == "--file") {
            if (argc <= i + 1) {
                usage(argv[0]);
                return 1;
            }
            file_path = std::string(argv[i + 1]);
            i++;
        }
    }
    if (file_path.size() == 0) {
        usage(argv[0]);
        return 1;
    }

    MediaImporter importer;
    if (!importer.open(file_path)) {
        return 1;
    }

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        base::LogError() << "SDL init failed";
        return -1;
    }

    SDL_Window *window = SDL_CreateWindow("MAVPlayer", 0, SDL_WINDOWPOS_CENTERED, kWindowWidth,
                                          kWindowHeight, SDL_WINDOW_SHOWN);
    SDL_Rect rect;
    rect.x = 0;
    rect.y = 0;
    rect.w = kWindowWidth;
    rect.h = kWindowHeight;

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    if (renderer == NULL) {
        base::LogError() << "Cannot create render";
        return 1;
    }
    Uint32 pixel_format = SDL_PIXELFORMAT_IYUV;
    SDL_Texture *texture =
        SDL_CreateTexture(renderer, pixel_format, SDL_TEXTUREACCESS_STREAMING, rect.w, rect.h);
    if (texture == NULL) {
        base::LogError() << "Create texture failed";
        return 1;
    }

    bool isquit = false;
    SDL_Event event;

    importer.start_working();

    while (!isquit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isquit = true;
            }
        }

        media_base::RawVideoFrame *video_frame = importer.read_frame();
        if (video_frame != nullptr) {
            SDL_UpdateYUVTexture(texture, NULL,
                                 (const Uint8 *)video_frame->frame_buffers[0]->frame_data,
                                 video_frame->frame_buffers[0]->line_size,
                                 (const Uint8 *)video_frame->frame_buffers[1]->frame_data,
                                 video_frame->frame_buffers[1]->line_size,
                                 (const Uint8 *)video_frame->frame_buffers[2]->frame_data,
                                 video_frame->frame_buffers[2]->line_size);
            delete video_frame;

            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, texture, NULL, &rect);
            SDL_RenderPresent(renderer);
            SDL_Delay(5);
        } else {
            SDL_Delay(1);
        }
    }
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

void usage(const char *bin_name) {
    std::cout << "Usage: " << bin_name << '\n'
              << '\n'
              << '\n'
              << "Options:" << '\n'
              << "  -h | --help     : show this help" << '\n'
              << "  -v | --version  : show version information " << '\n'
              << "  -f | --file     : set the play file path\n";
}
