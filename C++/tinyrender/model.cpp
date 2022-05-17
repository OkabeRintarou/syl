#include "model.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

Model::Model(const char *filename) {
    std::ifstream fin;
    fin.open(filename, std::ios::in);
    if (fin.fail()) {
        return;
    }
    std::string line;
    while (!fin.eof()) {
        std::getline(fin, line);
        std::istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v ")) {
            iss >> trash;
            Vec3f v;
            for (int i = 0; i < 3; i++) {
                iss >> v.raw[i];
            }
            verts_.emplace_back(v);
        } else if (!line.compare(0, 2, "f ")) {
            std::vector<int> f;
            int itrash, idx;
            iss >> trash;
            while (iss >> idx >> trash >> itrash >> trash >> itrash) {
                idx--;
                f.push_back(idx);
            }
            faces_.emplace_back(std::move(f));
        }
    }
    std::cerr << "# v# " << verts_.size() << " f# " << faces_.size() << std::endl;
}

Model::~Model() {}

std::size_t Model::nverts() const {
    return verts_.size();
}

std::size_t Model::nfaces() const {
    return faces_.size();
}

const std::vector<int> &Model::face(int i) const {
    return faces_[i];
}

const Vec3f &Model::vert(int i) const {
    return verts_[i];
}