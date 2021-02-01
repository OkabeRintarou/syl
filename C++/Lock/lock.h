#pragma once

class Lock {
public:
    explicit Lock() = default;
    virtual ~Lock() = default;

    virtual void lock() = 0;
    virtual void unlock() = 0;
};
