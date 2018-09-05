#!/usr/bin/python3
# -*- coding:utf-8 -*-


def sort_len(arr):
    no_min_index = -1
    min_value = arr[-1]
    for i in range(len(arr) - 2, -1, -1):
        if arr[i] > min_value:
            no_min_index = i
        else:
            min_value = min(min_value, arr[i])

    if no_min_index < 0:
        return 0
    no_max_index = -1
    max_value = arr[0]
    for i in range(1, len(arr)):
        if arr[i] < max_value:
            no_max_index = i
        else:
            max_value = max(max_value, arr[i])
    return no_max_index - no_min_index + 1


if __name__ == '__main__':
    print(sort_len([1, 5, 3, 4, 2, 6, 7]))
