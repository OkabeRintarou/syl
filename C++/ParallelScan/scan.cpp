#include "scan.h"
#include <cassert>
#include <cstdio>
#include <thread>
#include <omp.h>

void serial_scan(float *output, const float *input, const int len) {
    if (len == 0) return;

    output[0] = 0.0f;
    for (int i = 1; i < len; i++) {
        output[i] = output[i - 1] + input[i - 1];
    }
}

void parallel_cpu_scan_version_1(float *output, const float *input, const int len) {
    assert(!(len & (len - 1)));

    int i;

    #pragma omp parallel for schedule(static, 32)
    for (i = 0; i < len; i++) {
        output[i] = input[i];
    }

    /* up sweep phase */
    for (int d = 2; d <= len; d *= 2) {
        int sd = d / 2;

        #pragma  omp parallel for schedule(static)
        for (i = 0; i < len; i += d) {
            output[i  + d - 1] += output[i + sd - 1];
        }
    }

    /* down sweep phase */
    output[len - 1] = 0.0f;
    for (int d = len; d >= 2; d /= 2) {
        int sd = d / 2;

        #pragma omp parallel for schedule(static)
        for (i = 0; i < len; i += d) {
            float t = output[i + sd - 1];
            output[i + sd - 1] = output[i + d - 1];
            output[i + d - 1] += t;
        }
    }
}

void parallel_cpu_scan_version_2(float *output, const float *input, const int len) {
    if (len <= 32) {
        serial_scan(output, input, len);
        return;
    }

    const int mid = len / 2;
    {
        std::thread t1(serial_scan, output, input, mid);
        std::thread t2(serial_scan, output + mid, input + mid, len - mid);
        t1.join();
        t2.join();
    }
    {
        float base = output[mid - 1] + input[mid - 1];
        const auto f = [](float *vec, int len, float b) {
            for (int i = 0; i < len; i++) {
                vec[i] += b;
            }
        };
        const int mid_mid = (len - mid) / 2;
        std::thread t1(f, output + mid, mid_mid, base);
        std::thread t2(f, output + mid + mid_mid, len - mid - mid_mid, base);
        t1.join();
        t2.join();
    }
}
