#pragma once

#include "vec.h"
#include <vector>

class Model {
public:
    Model(const char *filename);
    ~Model();
    std::size_t nverts() const;
    std::size_t nfaces() const;
    const Vec3f &vert(int i) const;
    const std::vector<int> &face(int i) const;

private:
    std::vector<Vec3f> verts_;
    std::vector<std::vector<int>> faces_;
};