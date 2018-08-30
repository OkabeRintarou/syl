#!/usr/bin/python3
# -*- coding:utf-8 -*-


def shortest_path(matrix):
    m, n = len(matrix), len(matrix[0])
    for i in range(1, m):
        matrix[0][i] += matrix[0][i - 1]
    for i in range(1, n):
        matrix[i][0] += matrix[i - 1][0]
    for i in range(1, m):
        for j in range(1, n):
            matrix[i][j] += min(matrix[i - 1][j], matrix[i][j - 1])
    return matrix[m - 1][n - 1]


if __name__ == '__main__':
    m = [[1, 3, 5, 9], [8, 1, 3, 4], [5, 0, 6, 1], [8, 8, 4, 0]]
    print(shortest_path(m))
