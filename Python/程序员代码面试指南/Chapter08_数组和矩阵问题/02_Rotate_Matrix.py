#!/usr/bin/python3
# -*- coding:utf-8 -*-


def rotate_matrix(m):
    i, j = 0, len(m) - 1
    while i < j:
        for k in range(i, j):
            tmp = m[i][k]
            m[i][k] = m[j - k + i][i]
            m[j - k + i][i] = m[j][j - k + i]
            m[j][j - k + i] = m[k][j]
            m[k][j] = tmp
        i += 1
        j -= 1


def print_matrix(m):
    for i in m:
        for j in i:
            print(j, end=' ')
        print()


if __name__ == '__main__':
    matrix = [[1, 2, 3, 4], [5, 6, 7, 8], [9, 10, 11, 12], [13, 14, 15, 16]]
    rotate_matrix(matrix)
    print_matrix(matrix)
