#pragma once

#include "SDL2/SDL.h"

class Context;

using RenderFunc = void (*)(Context *, void *);

class Context {
public:
    Context(SDL_Window *w, SDL_Renderer *r, int wd, int ht) : window(w), render(r), width(wd), height(ht) {
        texture_ = SDL_CreateTexture(render, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height);
    }
    ~Context();

    Context(Context &&) = delete;
    Context &operator=(Context &&) = delete;
    Context(Context &) = delete;
    Context &operator=(const Context &) = delete;

    void set_render_func(RenderFunc func) { render_func_ = func; }
    void run(void * = nullptr);

    void begin_draw();
    void end_draw();
    // original point is left-top
    void draw_pixel(int x, int y);
    // original point is left-bottom
    void draw_pixel2(int x, int y);

    SDL_Window *window = nullptr;
    SDL_Renderer *render = nullptr;
    const int width;
    const int height;

private:
    void *pixel_ = nullptr;
    int pitch_;
    SDL_Texture *texture_;
    RenderFunc render_func_ = nullptr;
};
