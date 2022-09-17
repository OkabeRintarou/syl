#include "cuda_runtime.h"
#include "gpu_scan.cuh"
#include "scan.h"
#include "timer.h"
#include "util.h"
#include <iostream>
#include <memory>
#include <algorithm>
#include <omp.h>

static constexpr int ITER_COUNT = 5;

template<typename T>
static void verify(const T *expect, const T *actual, const int n) {
    for (int i = 0; i < n; i++) {
        if (actual[i] != expect[i]) {
            fprintf(stderr, "incorrect verify: [%d] expect: %d, actual: %d\n",
                    i, expect[i], actual[i]);
            break;
        }
    }
}

template<typename T>
static void prepare_cuda_scan(T *cpu_in, int n, T *&gpu_in, T *&gpu_out) {
    const int bytes = sizeof(T) * n;

    cudaMalloc(&gpu_in, bytes);
    cudaMalloc(&gpu_out, bytes);
    cudaMemcpy(gpu_in, cpu_in, bytes, cudaMemcpyHostToDevice);
}

template<typename T>
static T *test_cuda_scan_version_1(T *cpu_in, int n) {
    T *gpu_in, *gpu_out;
    prepare_cuda_scan(cpu_in, n, gpu_in, gpu_out);
    cuda_scan_version_1<<<1, 512, sizeof(int) * n>>>(gpu_out, gpu_in, n);
    cudaDeviceSynchronize();
    cudaError_t err = cudaGetLastError();
    if (err != cudaSuccess) {
        fprintf(stderr, "cuda scan version 1, error %d:%s\n",
                static_cast<int>(err),
                cudaGetErrorString(err));
        cudaFree(gpu_in);
        cudaFree(gpu_out);
        return nullptr;
    }

    T *cpu_out = new T[n];
    cudaMemcpy(cpu_out, gpu_out, sizeof(T) * n, cudaMemcpyDeviceToHost);

    cudaFree(gpu_in);
    cudaFree(gpu_out);
    return cpu_out;
}

template<typename T>
static T *test_cuda_scan_avoid_bank_conflict(T *cpu_in, int n) {
    T *gpu_in, *gpu_out;
    prepare_cuda_scan(cpu_in, n, gpu_in, gpu_out);
    cuda_scan_avoid_bank_conflict<<<1, 512, sizeof(T) * (n + 32)>>>(gpu_out, gpu_in, n);
    cudaDeviceSynchronize();
    cudaError_t err = cudaGetLastError();
    if (err != cudaSuccess) {
        fprintf(stderr, "cuda scan version 1, error %d:%s\n",
                static_cast<int>(err),
                cudaGetErrorString(err));
        cudaFree(gpu_in);
        cudaFree(gpu_out);
        return nullptr;
    }

    T *cpu_out = new T[n];
    cudaMemcpy(cpu_out, gpu_out, sizeof(T) * n, cudaMemcpyDeviceToHost);

    cudaFree(gpu_in);
    cudaFree(gpu_out);
    return cpu_out;
}

template<typename T>
static T *test_cuda_scan(T *cpu_in, int n) {
    T *cpu_out = new T[n];
    cuda_scan(cpu_out, cpu_in, n);
    return cpu_out;
}

int main() {
    for (int i = 0; i < 100; i++) {
        int size = 10243212;
        int *input = gen_int(size);
        int *output1 = new int[size];
        serial_scan(output1, input, size);
        int *output2 = test_cuda_scan(input, size);
        verify(output1, output2, size);
        delete []output1;
        delete []output2;
    }

    return 0;
}
