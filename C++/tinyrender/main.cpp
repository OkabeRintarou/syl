#include "model.h"
#include "sdl_helper.h"
#include <iostream>
#include <random>
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

static inline int orient(Vec2i a, Vec2i b, Vec2i c) {
    Vec2i ac = c - a;
    Vec2i bc = c - b;
    int cross_produce = ac.x * bc.y - ac.y * bc.x;
    return cross_produce >= 0 ? 1 : -1;
}

static bool in_triangle(int x, int y, Vec2i t0, Vec2i t1, Vec2i t2) {
    int r1 = orient(t0, t1, {x, y});
    int r2 = orient(t1, t2, {x, y});
    int r3 = orient(t2, t0, {x, y});
    int sum = r1 + r2 + r3;
    return sum == 3 || sum == -3;
}

static void triangle(Vec2i t0, Vec2i t1, Vec2i t2, SDL_Color color, Context *ctx) {
    SDL_SetRenderDrawColor(ctx->render, color.r, color.g, color.b, color.a);

    int x_min = std::min(t0.x, std::min(t1.x, t2.x));
    int x_max = std::max(t0.x, std::max(t1.x, t2.x));
    int y_min = std::min(t0.y, std::min(t1.y, t2.y));
    int y_max = std::max(t0.y, std::max(t1.y, t2.y));

    for (int x = x_min; x <= x_max; ++x) {
        for (int y = y_min; y <= y_max; ++y) {

            if (in_triangle(x, y, t0, t1, t2)) {
                ctx->draw_pixel2(x, y);
            }
        }
    }
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

    for (std::size_t i = 0, ni = model.nfaces(); i < ni; i++) {
        auto &face = model.face(i);
        Vec2i screen_coords[3];

        for (std::size_t j = 0; j < 3; j++) {
            Vec3f world_coords = model.vert(face[j]);
            screen_coords[j] = Vec2i(static_cast<int>((world_coords.x + 1.f) * (float) width / 2.f),
                                     static_cast<int>((world_coords.y + 1.f) * (float) height / 2.f));
        }
        triangle(screen_coords[0], screen_coords[1], screen_coords[2], {(uint8_t) (rand() % 256), (uint8_t) (rand() % 256), (uint8_t) (rand() % 256), 255}, ctx);
    }
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

    context->set_render_func(model_render);
    context->run(&model);

    return 0;
}
