#pragma once

#include <cstdint>
#include <cassert>
#include <utility>
#include <iostream>
#include <fstream>
#include <string>
#include "Common.h"

class Stream {
public:
    Stream() = default;
    virtual ~Stream() = default;
    DISALLOW_COPYABLE(Stream);
    DISALLOW_MOVEABLE(Stream);

    virtual bool is_end() const = 0;
};

class InputStream : public Stream {
public:
    virtual bool read_u4(uint32_t &) = 0;
};

class FileInputStream : public InputStream {
public:
    explicit FileInputStream(std::ifstream &&fin) : fin_(std::move(fin)) {
        assert(fin_.is_open() && "file must already open");
    }

    bool is_end() const override { return fin_.eof(); }
    bool meet_error() const { return !msg_.empty(); }
    const std::string &message() const { return msg_; }

    bool read_u4(uint32_t &) override;
private:
    using int_type = std::ifstream::int_type;
    void skip_whitespace();
private:
    std::ifstream fin_;
    std::string msg_;
};
