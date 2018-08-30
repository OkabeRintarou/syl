#!/usr/bin/python3
# -*- coding:utf-8 -*-


def multi(m, n):
    return m[0] * n[0] + m[1] * n[2], m[0] * n[1] + m[1] * n[3], m[2] * n[0] + m[3] * n[2], m[2] * n[1] + m[3] * n[3]


def matrix_pow(m, n):
    if n == 1:
        return m
    if n & 1:
        return multi(m, matrix_pow(m, n - 1))
    return matrix_pow(multi(m, m), n // 2)


def fibonacci(n):
    if n < 0:
        raise ValueError('n must greater than 0')
    if n <= 2:
        return 1

    res = matrix_pow((1, 1, 1, 0), n - 2)
    return res[0] + res[2]


def step(n):
    if n < 1:
        return 0
    if n == 1:
        return 1
    return step(n - 1) + step(n - 2)


def cow(n):
    if n <= 4:
        return n
    return cow(n - 1) + cow(n - 3)


if __name__ == '__main__':
    print(fibonacci(8))
