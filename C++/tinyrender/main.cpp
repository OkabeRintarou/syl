#include "sdl_helper.h"
#include <iostream>

void default_render_func(Context *ctx) {
    auto render = ctx->render;
    SDL_SetRenderDrawColor(render, 0xff, 0x00, 0x00, 0xff);
    SDL_RenderClear(render);
}

int main(int argc, char *argv[]) {
    auto context = init(1920, 1080, "tiny-render");
    if (!context) {
        std::cerr << "Fail to init sdl context\n";
        return -1;
    }

    context->set_render_func(default_render_func);
    context->run();

    return 0;
}
