#include "model.h"
#include "sdl_helper.h"
#include <iostream>
#include <vector>

void line(int x1, int y1, int x2, int y2, Context *ctx) {

    if (abs(y1 - y2) < abs(x1 - x2)) {
        if (x1 > x2) {
            std::swap(x1, x2);
            std::swap(y1, y2);
        }
    } else if (y1 > y2) {
        std::swap(x1, x2);
        std::swap(y1, y2);
    }

    int dx = x2 - x1, dy = y2 - y1, x = x1, y = y1;
    const int uy = dy < 0 ? -1 : 1;
    const int ux = dx < 0 ? -1 : 1;
    int eps = 0;

    dx = abs(dx);
    dy = abs(dy);

    if (dy < dx) {
        for (x = x1; x <= x2; x++) {
            ctx->draw_pixel2(x, y);

            eps += dy;
            if ((eps << 1) >= dx) {
                y += uy;
                eps -= dx;
            }
        }
    } else {
        for (y = y1; y <= y2; y++) {

            ctx->draw_pixel2(x, y);

            eps += dx;
            if ((eps << 1) >= dy) {
                x += ux;
                eps -= dy;
            }
        }
    }
}

static void triangle(Vec2i t0, Vec2i t1, Vec2i t2, SDL_Color color, Context *ctx) {
    SDL_SetRenderDrawColor(ctx->render, color.r, color.g, color.b, color.a);
    line(t0.x, t0.y, t1.x, t1.y, ctx);
    line(t1.x, t1.y, t2.x, t2.y, ctx);
    line(t2.x, t2.y, t0.x, t0.y, ctx);
}

void triangle_test(Context *ctx, void *) {
    Vec2i t0[3] = {{10, 70}, {50, 160}, {70, 80}};
    Vec2i t1[3] = {{180, 50}, {150, 1}, {70, 180}};
    Vec2i t2[3] = {{180, 150}, {120, 160}, {130, 180}};

    triangle(t0[0], t0[1], t0[2], {255, 0, 0, 255}, ctx);
    triangle(t1[0], t1[1], t1[2], {255, 255, 255, 255}, ctx);
    triangle(t2[0], t2[1], t2[2], {0, 255, 0, 255}, ctx);
}

void model_render(Context *ctx, void *d) {
    Model &model = *((Model *) d);
    const auto width = float(ctx->width);
    const auto height = float(ctx->height);

    SDL_SetRenderDrawColor(ctx->render, 0xff, 0x00, 0x00, 0xff);

    ctx->begin_draw();
    for (std::size_t i = 0, ni = model.nfaces(); i < ni; i++) {
        auto &face = model.face(i);
        for (std::size_t j = 0; j < 3; j++) {
            const Vec3f &v0 = model.vert(face[j]);
            const Vec3f &v1 = model.vert(face[(j + 1) % 3]);
            int x0 = static_cast<int>((v0.x + 1.0f) * width / 2.0f);
            int y0 = static_cast<int>((v0.y + 1.0f) * height / 2.0f);
            int x1 = static_cast<int>((v1.x + 1.0f) * width / 2.0f);
            int y1 = static_cast<int>((v1.y + 1.0f) * height / 2.0f);
            line(x0, y0, x1, y1, ctx);
        }
    }
    ctx->end_draw();
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "usage: " << argv[0] << " <obj-file>" << std::endl;
        return -1;
    }
    Model model(argv[1]);
    if (model.nverts() == 0) {
        std::cerr << "fail to load model " << argv[1] << std::endl;
        return -1;
    }

    auto context = init(1920, 1080, "tiny-render");
    if (!context) {
        std::cerr << "Fail to init sdl context\n";
        return -1;
    }

    context->set_render_func(triangle_test);
    context->run(&model);

    return 0;
}
