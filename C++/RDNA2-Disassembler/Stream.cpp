#include "Stream.h"

bool FileInputStream::read_u4(uint32_t &val) {
    skip_whitespace();
    if (is_end())
        return false;
    std::string str;
    str.reserve(16);

    int_type c;
    while (!is_end()) {
        c = fin_.get();
        if (!std::isspace(c))
            str.push_back(c);
        else
            break;
    }
    if (str.empty())
        return false;

    try {
        unsigned long v = std::stoul(str, nullptr, 16);
        val = static_cast<uint32_t>(v);
        return true;
    } catch (const std::exception &e) {
        msg_ = e.what();
        return false;
    }
}

void FileInputStream::skip_whitespace() {
    int_type c;
    while (!is_end()) {
        c = fin_.peek();
        if (std::isspace(c)) {
            fin_.get();
            continue;
        } else
            break;
    }
}
