#include <algorithm>
#include <bitset>
#include <cstdint>
#include <iostream>
#include <vector>

#include "SDL2/SDL.h"
#include "base/log.h"
#include "base/unused.h"

int main(int argc, const char *argv[]) {
    UNUSED(argc);
    UNUSED(argv);
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        base::LogError() << "SDL init failed";
        return -1;
    }

    const int windowWidth = 1280;
    const int windowHeight = 720;

    SDL_Window *window = SDL_CreateWindow("MAVPlayer", 0, SDL_WINDOWPOS_CENTERED, windowWidth,
                                          windowHeight, SDL_WINDOW_SHOWN);

    SDL_Renderer *render = SDL_CreateRenderer(window, -1, 0);
    bool isquit = false;
    SDL_Event event;

    while (!isquit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isquit = true;
            }
        }

        SDL_RenderPresent(render);
        SDL_Delay(30);
    }
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
