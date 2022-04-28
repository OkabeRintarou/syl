#include "scan.h"
#include "timer.h"
#include "util.h"
#include <iostream>
#include <memory>

static constexpr int ITER_COUNT = 5;

int main() {
    double elapsed1, elapsed2, elapsed3;
    const int size = (8 << 20);
    Timer timer;
    float *input, *output1, *output2, *output3;

    output1 = new float[size];
    output2 = new float[size];
    output3 = new float[size];
    elapsed1 = elapsed2 = elapsed3 = 0.0;

    for (int i = 0; i < ITER_COUNT; i++) {
        input = gen(size);

        timer.reset();
        serial_scan(output1, input, size);
        elapsed1 += timer.elapsed_milliseconds();

        timer.reset();
        parallel_cpu_scan_version_1(output2, input, size);
        elapsed2 += timer.elapsed_milliseconds();

        timer.reset();
        parallel_cpu_scan_version_2(output3, input, size);
        elapsed3 += timer.elapsed_milliseconds();

        // verify correctness
        for (int j = 0; j < size; j++) {
            if (fabsf(output1[j] - output2[j]) > 1000.0f) {
                fprintf(stderr, "parallel cpu scan version 1: [%d] expect: %f, actual: %f\n", j, output1[j], output2[j]);
                break;
            }
            if (fabsf(output1[j] - output3[j]) > 1000.0f) {
                fprintf(stderr, "parallel cpu scan version 1: [%d] expect: %f, actual: %f\n", j, output1[j], output2[j]);
                fprintf(stderr, "parallel cpu scan version 2: [%d] expect: %f, actual: %f\n", j, output1[j], output2[j]);
                break;
            }
        }
        delete[] input;
    }

    elapsed1 /= static_cast<double>(ITER_COUNT);
    elapsed2 /= static_cast<double>(ITER_COUNT);
    elapsed3 /= static_cast<double>(ITER_COUNT);

    printf("%-25s: %.2f ms\n", "serial scan", elapsed1);
    printf("%-25s: %.2f ms\n", "parallel cpu scan(V1)", elapsed2);
    printf("%-25s: %.2f ms\n", "parallel cpu scan(V2)", elapsed3);

    return 0;
}
