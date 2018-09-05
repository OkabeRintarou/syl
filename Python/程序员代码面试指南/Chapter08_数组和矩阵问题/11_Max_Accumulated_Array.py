#!/usr/bin/python3
# -*- coding:utf-8 -*-
import sys


def max_accumulated_sum(arr):
    cur, m = 0, -sys.maxsize - 1
    for v in arr:
        cur += v
        m = max(m, cur)
        if cur < 0:
            cur = 0
    return m


if __name__ == '__main__':
    print(max_accumulated_sum([1, -2, 3, 5, -2, 6, -1]))
