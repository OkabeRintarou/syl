#pragma once
#include <cstdio>
#include <cuda_runtime.h>

template<typename T>
__global__ void
cuda_scan_version_1(T *output, const T *input, const int n) {
    extern __shared__ T temp[];

    int tid = threadIdx.x;
    int offset = 1;

    temp[2 * tid] = input[2 * tid];
    temp[2 * tid + 1] = input[2 * tid + 1];

    for (int d = n >> 1; d > 0; d >>= 1) {
        __syncthreads();

        if (tid < d) {
            int ai = offset * (2 * tid + 1) - 1;
            int bi = offset * (2 * tid + 2) - 1;
            temp[bi] += temp[ai];
        }

        offset *= 2;
    }

    if (tid == 0) {
        temp[n - 1] = T();
    }

    for (int d = 1; d < n; d *= 2) {
        offset >>= 1;
        __syncthreads();

        if (tid < d) {
            int ai = offset * (2 * tid + 1) - 1;
            int bi = offset * (2 * tid + 2) - 1;
            T t = temp[ai];
            temp[ai] = temp[bi];
            temp[bi] += t;
        }
    }

    __syncthreads();

    output[2 * tid] = temp[2 * tid];
    output[2 * tid + 1] = temp[2 * tid + 1];
}

template<typename T>
__global__ void
cuda_scan_avoid_bank_conflict(T *output, const T *input, const int n) {
#define NUM_BANKS 32
#define LOG_NUM_BANKS 5

#ifdef ZERO_BACK_CONFLICTS
#define CONFLICT_FREE_OFFSET(n) ((n) >> NUM_BANKS + (n) >> (2 * LOG_NUM_BANKS))
#else
#define CONFLICT_FREE_OFFSET(n) ((n) >> LOG_NUM_BANKS)
#endif

    extern __shared__ T temp[];

    int tid = threadIdx.x;
    int offset = 1, ai, bi, bank_offset_a, bank_offset_b, xi, yi;

    ai = tid;
    bi = tid + n / 2;

    bank_offset_a = CONFLICT_FREE_OFFSET(ai);
    bank_offset_b = CONFLICT_FREE_OFFSET(bi);

    temp[ai + bank_offset_a] = input[ai];
    temp[bi + bank_offset_b] = input[bi];

    for (int d = n >> 1; d > 0; d >>= 1) {
        __syncthreads();

        if (tid < d) {
            xi = offset * (2 * tid + 1) - 1;
            yi = offset * (2 * tid + 2) - 1;
            xi += CONFLICT_FREE_OFFSET(xi);
            yi += CONFLICT_FREE_OFFSET(yi);
            temp[yi] += temp[xi];
        }

        offset *= 2;
    }

    if (tid == 0) {
        temp[n - 1 + CONFLICT_FREE_OFFSET(n - 1)] = T();
    }

    for (int d = 1; d < n; d *= 2) {
        offset >>= 1;
        __syncthreads();

        if (tid < d) {
            xi = offset * (2 * tid + 1) - 1;
            yi = offset * (2 * tid + 2) - 1;
            xi += CONFLICT_FREE_OFFSET(xi);
            yi += CONFLICT_FREE_OFFSET(yi);
            T t = temp[xi];
            temp[xi] = temp[yi];
            temp[yi] += t;
        }
    }

    __syncthreads();

    output[ai] = temp[ai + bank_offset_a];
    output[bi] = temp[bi + bank_offset_b];
#undef NUM_BANKS
#undef LOG_NUM_BANKS
#undef CONFLICT_FREE_OFFSET
}

template<typename T>
__global__ void
cuda_scan_aux(T *output, const T *input, const int n, T *sum_out, const int blk_idx) {
#define NUM_BANKS 32
#define LOG_NUM_BANKS 5

#ifdef ZERO_BACK_CONFLICTS
#define CONFLICT_FREE_OFFSET(n) ((n) >> NUM_BANKS + (n) >> (2 * LOG_NUM_BANKS))
#else
#define CONFLICT_FREE_OFFSET(n) ((n) >> LOG_NUM_BANKS)
#endif

    extern __shared__ T temp[];

    int tid = threadIdx.x;
    if (tid >= n) return;

    int offset = 1, ai, bi, bank_offset_a, bank_offset_b, xi, yi;

    ai = tid;
    bi = tid + n / 2;

    bank_offset_a = CONFLICT_FREE_OFFSET(ai);
    bank_offset_b = CONFLICT_FREE_OFFSET(bi);

    temp[ai + bank_offset_a] = input[ai];
    temp[bi + bank_offset_b] = input[bi];

    for (int d = n >> 1; d > 0; d >>= 1) {
        __syncthreads();

        if (tid < d) {
            xi = offset * (2 * tid + 1) - 1;
            yi = offset * (2 * tid + 2) - 1;
            xi += CONFLICT_FREE_OFFSET(xi);
            yi += CONFLICT_FREE_OFFSET(yi);
            temp[yi] += temp[xi];
        }

        offset *= 2;
    }

    if (tid == 0) {
        if (sum_out != nullptr) {
            sum_out[blk_idx] = temp[n - 1 + CONFLICT_FREE_OFFSET(n - 1)];
        }
        temp[n - 1 + CONFLICT_FREE_OFFSET(n - 1)] = T();
    }

    for (int d = 1; d < n; d *= 2) {
        offset >>= 1;
        __syncthreads();

        if (tid < d) {
            xi = offset * (2 * tid + 1) - 1;
            yi = offset * (2 * tid + 2) - 1;
            xi += CONFLICT_FREE_OFFSET(xi);
            yi += CONFLICT_FREE_OFFSET(yi);
            T t = temp[xi];
            temp[xi] = temp[yi];
            temp[yi] += t;
        }
    }

    __syncthreads();

    output[ai] = temp[ai + bank_offset_a];
    output[bi] = temp[bi + bank_offset_b];

#undef NUM_BANKS
#undef LOG_NUM_BANKS
#undef CONFLICT_FREE_OFFSET
}

template<typename T>
__global__ void vector_add(T *in, const int n, T *val, const int blk_idx) {
    int tid = threadIdx.x;
    if (tid >= n) return;

    in[tid] += val[blk_idx];
}

template<typename T>
__global__ void gpu_serial_scan(T *output, const T *in, const int n) {
    output[0] = 0;
    for (int i = 1; i < n; i++) {
        output[i] = in[i - 1] + output[i - 1];
    }
}

template<typename T>
void cuda_scan(T *output, const T *input, const int n) {
    const int threads_per_block = 256;
    const int data_delta = threads_per_block << 1;
    const int block_cnt = (n + data_delta - 1) / data_delta;
    T *sum_aux, *exc_sum, *gpu_in, *gpu_out;

    cudaMalloc((void**)&sum_aux, sizeof(T) * block_cnt);
    cudaMalloc((void**)&exc_sum, sizeof(T) * block_cnt);
    cudaMalloc((void**)&gpu_in, sizeof(T) * block_cnt * data_delta);
    cudaMalloc((void**)&gpu_out, sizeof(T) * block_cnt * data_delta);
    cudaMemset(gpu_in + (block_cnt - 1) * data_delta, 0, sizeof(T) * data_delta);
    cudaMemcpy(gpu_in, input, sizeof(T) * n, cudaMemcpyHostToDevice);

    for (int i = 0; i < block_cnt; i++) {
        cuda_scan_aux<<<1, threads_per_block, sizeof(T) * (data_delta + 32)>>>(gpu_out + i * data_delta, gpu_in + i * data_delta, data_delta, sum_aux, i);


    }

    // TODO: opt this function
    gpu_serial_scan<<<1, 1>>>(exc_sum, sum_aux, block_cnt);

    for (int i = 1; i < block_cnt; i++) {
        vector_add<<<1, data_delta>>>(gpu_out + i * data_delta, data_delta, exc_sum, i);
    }

    cudaMemcpy(output, gpu_out, sizeof(T) * n, cudaMemcpyDeviceToHost);
    cudaFree(gpu_in);
    cudaFree(gpu_out);
    cudaFree(sum_aux);
    cudaFree(exc_sum);
}