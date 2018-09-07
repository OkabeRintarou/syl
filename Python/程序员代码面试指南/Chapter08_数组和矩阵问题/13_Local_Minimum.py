#!/usr/bin/python3
# -*- coding:utf-8 -*-


def local_minimum(arr):
    n = len(arr)
    if not arr or not n:
        return -1
    if n <= 2:
        return 0
    if arr[n - 1] < arr[n - 2]:
        return n - 1
    left, right = 1, n - 2
    while left < right:
        mid = ((right - left) >> 1) + left
        if arr[mid] > arr[mid - 1]:
            right = mid - 1
        elif arr[mid] > arr[mid + 1]:
            left = mid + 1
        else:
            return mid
    return left


if __name__ == '__main__':
    print(local_minimum([100, 99, 8, 9, 7, 10, 20]))
