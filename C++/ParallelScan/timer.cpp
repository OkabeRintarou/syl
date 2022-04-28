#include "timer.h"

using namespace std::chrono;

void Timer::reset() { start_ = high_resolution_clock::now(); }

double Timer::elapsed_nanoseconds() const {
    const auto l =
        duration_cast<nanoseconds>(high_resolution_clock::now() - start_).count();
    return static_cast<double>(l);
}

double Timer::elapsed_microseconds() const {
    double ns = elapsed_nanoseconds();
    return ns / 1000.0;
}

double Timer::elapsed_milliseconds() const {
    double micro = elapsed_microseconds();
    return micro / 1000.0;
}

double Timer::elapsed_seconds() const {
    double ms = elapsed_milliseconds();
    return ms / 1000.0;
}
