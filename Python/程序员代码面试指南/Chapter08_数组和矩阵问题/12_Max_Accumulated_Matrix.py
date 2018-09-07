#!/usr/bin/python3
# -*- coding:utf-8 -*-
import sys


def max_accumulated_matrix(matrix):
    if not matrix or len(matrix) == 0 or len(matrix[0]) == 0:
        return 0
    row, col = len(matrix), len(matrix[0])
    m = -sys.maxsize - 1
    for i in range(row):
        s = [0 for _ in range(col)]
        for j in range(i, row):
            cur = 0
            for k in range(row):
                s[k] += matrix[j][k]
                cur += s[k]
                m = max(m, cur)
                if cur < 0:
                    cur = 0
    return m


if __name__ == '__main__':
    print(max_accumulated_matrix([[-90, 48, 78], [64, -40, 64], [-81, -7, 66]]))
