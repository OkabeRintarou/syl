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
    if (len <= (1 << 20)) {
        return serial_scan(output, input, len);
    }

    int mid = len / 2;
    #pragma omp parallel num_threads(2)
    {
        int thread_id = omp_get_thread_num();
        const float *in_begin = input + thread_id * mid;
        float *out_begin = output + thread_id * mid;

        int n = mid;
        const float *in_end = in_begin + mid;
        if (in_end > input + len) {
            in_end = input + len;
            n = in_end - in_begin;
        }
        parallel_cpu_scan_version_2(out_begin, in_begin, n);
    }

    const float base = output[mid - 1] + input[mid - 1];

    #pragma omp parallel for schedule(static)
    for (int i = mid; i < len; i++) {
        output[i] += base;
    }
}
