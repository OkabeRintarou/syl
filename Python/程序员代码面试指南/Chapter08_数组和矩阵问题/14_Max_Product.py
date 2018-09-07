#!/usr/bin/python3
# -*- coding:utf-8 -*-


def max_product(arr):
    if not arr or not len(arr):
        return 0
    max_end, min_end, ans = arr[0], arr[0], arr[0]
    for v in arr[1:]:
        a, b, c = max_end * v, min_end * v, v
        max_end = max(a, b, c)
        min_end = min(a, b, c)
        ans = max(max_end, ans)
    return ans


if __name__ == '__main__':
    print(max_product([3, 4, 5]))
    print(max_product([-3, 4, -5]))
    print(max_product([0.1, 0.1, 100]))
    print(max_product([-2.5, 4, 0, 3, 0.5, 8, -1]))
