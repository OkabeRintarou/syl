#include "model.h"
#include "sdl_helper.h"
#include <iostream>
#include <vector>

void line(int x1, int y1, int x2, int y2, SDL_Renderer *render) {

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
            SDL_RenderDrawPoint(render, x, y);

            eps += dy;
            if ((eps << 1) >= dx) {
                y += uy;
                eps -= dx;
            }
        }
    } else {
        for (y = y1; y <= y2; y++) {
            SDL_RenderDrawPoint(render, x, y);

            eps += dx;
            if ((eps << 1) >= dy) {
                x += ux;
                eps -= dy;
            }
        }
    }
}

void bresenham_render_test(Context *ctx, void *) {
    auto render = ctx->render;
    const int width = ctx->width;
    const int height = ctx->height;
    const int origin_x = width / 2;
    const int origin_y = height / 2;
    float ratio[] = {0.0f, 0.1f, 0.25f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.75f, 0.8f, 0.9f, 1.0f};
    std::vector<int> end_points;
    end_points.reserve(64);

    for (auto i = std::begin(ratio), ei = std::end(ratio); i < ei; ++i) {
        for (auto j = std::begin(ratio), ej = std::end(ratio); j < ej; ++j) {
            end_points.emplace_back(static_cast<int>(*i * (float) width));
            end_points.emplace_back(static_cast<int>(*j * (float) height));
        }
    }

    SDL_SetRenderDrawColor(render, 0xff, 0x00, 0x00, 0xff);
    std::size_t points_cnt = end_points.size();
    for (std::size_t i = 0; i < points_cnt; i += 2) {
        line(origin_x, origin_y, end_points[i], end_points[i + 1], render);
    }
}

void model_render(Context *ctx, void *d) {
    Model &model = *((Model *) d);
    const auto width = float(ctx->width);
    const auto height = float(ctx->height);

    SDL_SetRenderDrawColor(ctx->render, 0xff, 0x00, 0x00, 0xff);

    for (std::size_t i = 0, ni = model.nfaces(); i < ni; i++) {
        auto &face = model.face(i);
        for (std::size_t j = 0; j < 3; j++) {
            const Vec3f &v0 = model.vert(face[j]);
            const Vec3f &v1 = model.vert(face[(j + 1) % 3]);
            int x0 = static_cast<int>((v0.x + 1.0f) * width / 2.0f);
            int y0 = static_cast<int>((v0.y + 1.0f) * height / 2.0f);
            int x1 = static_cast<int>((v1.x + 1.0f) * width / 2.0f);
            int y1 = static_cast<int>((v1.y + 1.0f) * height / 2.0f);
            line(x0, y0, x1, y1, ctx->render);
        }
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
