#include <random>
#include "util.h"

float *gen_float(int size) {
    static std::mt19937 rnd(std::random_device{}());
    static std::uniform_real_distribution<float> dist(.0f, 0.5f);
    auto v = new float[size];
    for (int i = 0; i < size; i++) {
        v[i] = static_cast<float>(dist(rnd));
    }
    return v;
}

int *gen_int(int size) {
    static std::mt19937 rnd(std::random_device{}());
    static std::uniform_int_distribution<int> dist(0, 10);
    auto v = new int[size];
    for (int i = 0; i < size; i++) {
        v[i] = dist(rnd);
    }
    return v;
}
