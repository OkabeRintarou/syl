#!/usr/bin/python3
# -*- coding:utf-8 -*-


def left_unique(arr):
    n = len(arr)
    if not arr or not n:
        return
    u, i = 0, 1
    while i < n:
        if arr[i] == arr[u]:
            i += 1
        else:
            arr[u + 1], arr[i] = arr[i], arr[u + 1]
            u += 1
            i += 1


if __name__ == '__main__':
    arr = [1, 2, 2, 2, 3, 3, 4, 5, 6, 6, 7, 7, 8, 8, 8, 9]
    left_unique(arr)
    print(arr)
