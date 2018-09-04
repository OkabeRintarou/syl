#!/usr/bin/python3
# -*- coding:utf-8 -*-


def print_matrix(m):
    def print_edge(s, e):
        for k in range(s, e + 1):
            print(m[s][k], end=' ')
        for k in range(s + 1, e + 1):
            print(m[k][e], end=' ')
        for k in range(e - 1, s - 1, -1):
            print(m[e][k], end=' ')
        for k in range(e - 1, s, -1):
            print(m[k][s], end=' ')

    i, j = 0, len(m) - 1
    while i < j:
        print_edge(i, j)
        i += 1
        j -= 1


if __name__ == '__main__':
    print_matrix([[1, 2, 3, 4], [5, 6, 7, 8], [9, 10, 11, 12], [13, 14, 15, 16]])
