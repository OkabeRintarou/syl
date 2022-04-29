#include "scan.h"
#include "timer.h"
#include "util.h"
#include <cstdio>
#include <cstdlib>
#include <getopt.h>

static constexpr int default_size = (1 << 20);

static int pow_of_two(int a) {
    int n = a - 1;
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    return n + 1;
}

static void usage(const char *prog_name) {
    printf("usage: %s [-p] [-s N]\n", prog_name);
    exit(1);
}

int main(int argc, char *argv[]) {
    bool use_parallel_version = false;
    int N = -1;

    int c;
    opterr = 0;

    while ((c = getopt(argc, argv, "ps:")) != -1) {
        switch (c) {
            case 'p':
                use_parallel_version = true;
                break;
            case 's':
                N = atoi(optarg);
                break;
            default:
                usage(argv[0]);
                break;
        }
    }
    if (N < 0) {
        N = default_size;
    }

    N = pow_of_two(N);

    int *input = gen_int(N);
    int *output = new int[N];
    Timer timer;
    timer.reset();

    if (use_parallel_version) {
        serial_scan(output, input, N);
    } else {
        parallel_cpu_scan_version_1(output, input, N);
    }
    printf("%s elapsed %f ms\n",
           use_parallel_version ? "Parallel" : "Serial",
           timer.elapsed_milliseconds());
    delete[] output;

    return 0;
}