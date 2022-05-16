#include "context.h"
#include <cassert>
#include <iostream>

Context::~Context() {
    if (render) {
        SDL_DestroyRenderer(render);
        render = nullptr;
    }
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
}

void Context::run() {

    assert(window != nullptr && render != nullptr);

    SDL_Event event;
    bool quit = false;

    unsigned cur_time;
    unsigned next_frame_time = SDL_GetTicks();

    for (;;) {
        do {
            cur_time = SDL_GetTicks();
            SDL_PollEvent(&event);
            if (event.type == SDL_QUIT) {
                quit = true;
                break;
            }
            SDL_Delay(1);
        } while (cur_time < next_frame_time);

        if (quit) {
            break;
        }

        if (static_cast<int>(cur_time - next_frame_time) > 1000) {
            next_frame_time = cur_time + 1000 / 60;
        } else {
            next_frame_time += 1000 / 60;
        }

        SDL_SetRenderDrawColor(render, 0xff, 0xff, 0xff, 0xff);
        SDL_RenderClear(render);

        if (render_func_) {
            render_func_(this);
        }

        SDL_RenderPresent(render);
    }
}
