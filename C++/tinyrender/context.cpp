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

void Context::run(void *d) {

    assert(window != nullptr && render != nullptr);

    SDL_Event event;
    bool quit = false;

    unsigned cur_time, last_time = 0, delta_time;
    unsigned next_frame_time = SDL_GetTicks();
    float fps;

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
            render_func_(this, d);
        }

        SDL_RenderCopy(render, texture_, nullptr, nullptr);
        SDL_RenderPresent(render);

        cur_time = SDL_GetTicks();
        delta_time = cur_time - last_time;
        if (delta_time == 0) ++delta_time;
        fps = 1000.0f / static_cast<float>(delta_time);
        std::cout << "fps: " << fps << std::endl;

        last_time = cur_time;
    }
}

void Context::draw_pixel(int x, int y) {
    assert(pixel_ != nullptr);
    uint8_t r, g, b, a;
    SDL_GetRenderDrawColor(render, &r, &g, &b, &a);
    auto start = (uint32_t*)(static_cast<uint8_t*>(pixel_) + y * pitch_) + x;
    *start = (a << 24) | (r << 16) | (g << 8) | b;
}

void Context::draw_pixel2(int x, int y) {
    assert(pixel_ != nullptr);
    uint8_t r, g, b, a;
    SDL_GetRenderDrawColor(render, &r, &g, &b, &a);
    auto start = (uint32_t*)(static_cast<uint8_t*>(pixel_) + (height - y) * pitch_) + x;
    *start = (a << 24) | (r << 16) | (g << 8) | b;
}

void Context::begin_draw() {
    SDL_LockTexture(texture_, nullptr, &pixel_, &pitch_);
}

void Context::end_draw() {
    SDL_UnlockTexture(texture_);
    pixel_ = nullptr;
    pitch_ = 0;
}
