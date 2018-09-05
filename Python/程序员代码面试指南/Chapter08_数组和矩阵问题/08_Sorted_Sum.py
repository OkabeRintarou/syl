#!/usr/bin/python3
# -*- coding:utf-8 -*-


def sum2(arr, t):
    if not arr or len(arr) == 0:
        return []
    ans = []
    i, j = 0, len(arr) - 1
    while i < j:
        s = arr[i] + arr[j]
        if s == t:
            if not ans:
                ans.append((arr[i], arr[j]))
            elif not (arr[i], arr[j]) == ans[-1]:
                ans.append((arr[i], arr[j]))
            i += 1
            j -= 1
        elif s < t:
            i += 1
        else:
            j -= 1
    return ans


def sum3(arr, t):
    if not arr or len(arr) == 0:
        return []
    ans = []
    for i in range(len(arr)):
        rest = sum2(arr[i + 1:], t - arr[i])
        if not ans:
            for v in rest:
                ans.append((arr[i], v[0], v[1]))
        elif arr[i] != ans[-1][0]:
            for v in rest:
                ans.append((arr[i], v[0], v[1]))
    return ans


if __name__ == '__main__':
    print(sum2([-8, -4, -3, 0, 1, 2, 4, 5, 8, 9], 10))
    print(sum2([2, 2, 2, 5, 5], 7))
    print(sum3([-8, -4, -3, 0, 1, 2, 4, 5, 8, 9], 10))
    print(sum3([2, 2, 2, 5, 5], 6))
