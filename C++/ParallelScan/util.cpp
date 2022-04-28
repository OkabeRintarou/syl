#include <random>
#include "util.h"

float *gen(int size) {
    static std::mt19937 rnd(std::random_device{}());
    static std::uniform_real_distribution<float> dist(0, 1);
    auto v = new float[size];
    for (int i = 0; i < size; i++) {
        v[i] = static_cast<float>(dist(rnd));
    }
    return v;
}
