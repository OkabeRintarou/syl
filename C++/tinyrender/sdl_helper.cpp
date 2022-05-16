#include "sdl_helper.h"
#include "context.h"
#include <SDL2/SDL.h>

std::optional<Context> init(const int width, const int height, const char *title) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return std::nullopt;
    }
    atexit(SDL_Quit);

    SDL_Window *window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                          width, height, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        return std::nullopt;
    }

    SDL_Renderer *render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (render == nullptr) {
        SDL_DestroyWindow(window);
    }

    return std::optional<Context>(std::in_place, window, render, width, height);
}
