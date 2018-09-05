#!/usr/bin/python3
# -*- coding:utf-8 -*-
import sys


def integration(arr):
    if not arr or len(arr) == 0:
        return 0

    length = 0
    for i in range(len(arr)):
        maximum, minimum = -sys.maxsize - 1, sys.maxsize
        s = set()
        for j in range(i, len(arr)):
            if arr[j] in s:
                break
            maximum = max(maximum, arr[j])
            minimum = min(minimum, arr[j])
            if maximum - minimum == j - i:
                length = max(length, j - i + 1)

    return length


if __name__ == '__main__':
    print(integration([5, 5, 3, 2, 6, 4, 3]))
