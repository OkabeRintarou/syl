#!/usr/bin/python3
# -*- coding:utf-8 -*-


def max_sub_array(arr, t):
    if not arr or len(arr) == 0:
        return 0
    length = 0
    i, j, s = 0, 0, 0
    while j < len(arr):
        s += arr[j]
        if s == t:
            length = max(length, j - i + 1)
            i += 1
            s -= arr[i - 1]
        elif s < t:
            j += 1
        else:
            i += 1
            s -= arr[i - 1]
            if i > j:
                j += 1

    return length


def max_sub_array2(arr, t):
    ss, mp = [], {0: -1}
    s, length = 0, 0
    for i in range(len(arr)):
        s += arr[i]
        if s - t in mp:
            j = mp[s - t]
            length = max(length, i - j)
        if s not in mp:
            mp[s] = i
    return length


if __name__ == '__main__':
    print(max_sub_array([100, 2, 3], 5))
    print(max_sub_array2([1, 2, 3, 3], 6))
