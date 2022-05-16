#pragma once
#include "SDL2/SDL.h"

class Context;

using RenderFunc = void (*)(Context *);

class Context {
public:
    Context(SDL_Window *w, SDL_Renderer *r, int wd, int ht) : window(w), render(r), width(wd), height(ht) {}
    ~Context();

    Context(Context &&) = delete;
    Context &operator=(Context &&) = delete;
    Context(Context &) = delete;
    Context &operator=(const Context &) = delete;

    void set_render_func(RenderFunc func) { render_func_ = func; }
    void run();

    SDL_Window *window = nullptr;
    SDL_Renderer *render = nullptr;
    const int width;
    const int height;

private:
    RenderFunc render_func_ = nullptr;
};
