#!/usr/bin/python3
# -*- coding:utf-8 -*-


def matrix_find_k(m, k):
    row, col = len(m), len(m[0])
    i, j = 0, col - 1
    while i < row and col >= 0:
        if m[i][j] == k:
            return True
        elif m[i][j] < k:
            i += 1
        else:
            j -= 1
    return False


if __name__ == '__main__':
    matrix = [[0, 1, 2, 5], [2, 3, 4, 7], [4, 4, 4, 8], [5, 7, 7, 9]]
    print(matrix_find_k(matrix, 7))
    print(matrix_find_k(matrix, 6))
