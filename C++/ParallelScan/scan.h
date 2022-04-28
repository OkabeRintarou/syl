#pragma once

void serial_scan(float *output, const float *input, const int len);

void parallel_cpu_scan_version_1(float *output, const float *input, const int len);
void parallel_cpu_scan_version_2(float *output, const float *input, const int len);
