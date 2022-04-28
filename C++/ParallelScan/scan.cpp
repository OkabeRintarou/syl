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

    const int d = len / 2;

    #pragma omp parallel num_threads(2)
    {
        int thread_id = omp_get_thread_num();
        int start = thread_id * d;
        int end = start + d;
        if (end > len) {
            end = len;
        }
        serial_scan(output + start, input + start, end - start);
    }

    const float base = output[d - 1] + input[d - 1];

    #pragma omp parallel for schedule(static)
    for (int i = d; i < len; i++) {
        output[i] += base;
    }
}
