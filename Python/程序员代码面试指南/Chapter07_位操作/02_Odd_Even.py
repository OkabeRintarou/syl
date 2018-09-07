#!/usr/bin/python3
# -*- coding:utf-8 -*-


def odd_even_1(arr):
    v = arr[0]
    for i in arr[1:]:
        v ^= i
    return v


def odd_even_2(arr):
    v = arr[0]
    for i in arr[1:]:
        v ^= i
    k = v & (~v + 1)
    t = v
    for i in arr:
        if k & i:
            t ^= i
    return t, t ^ v


if __name__ == '__main__':
    print(odd_even_1([1, 1, 1, 1, 3, 6, 5, 3, 6]))
    print(odd_even_2([1, 1, 1, 1, 3, 6, 5, 3]))
