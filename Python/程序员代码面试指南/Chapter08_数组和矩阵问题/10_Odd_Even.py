#!/usr/bin/python3
# -*- coding:utf-8 -*-
import random


def odd_even(arr):
    if not arr or len(arr) < 2:
        return arr

    odd, even = 1, 0
    while odd < len(arr) and even < len(arr):
        if arr[-1] & 1:
            arr[-1],arr[odd] = arr[odd],arr[-1]
            odd += 2
        else:
            arr[-1],arr[even] = arr[even],arr[-1]
            even += 2
    return arr


if __name__ == '__main__':
    print(odd_even([random.randint(0, 30) for _ in range(20)]))
