#!/usr/bin/python3
# -*- coding:utf-8 -*-


def n2(arr):
    candidate, times = 0, 0
    for v in arr:
        if times == 0:
            candidate = v
            times += 1
        elif v == candidate:
            times += 1
        else:
            times -= 1
    times = 0
    for v in arr:
        if v == candidate:
            times += 1
    if times > len(arr) / 2:
        print(candidate)
    else:
        print('no such number')


def nk(arr, k):
    if k < 2:
        raise ValueError('the value of k is invalid')
    cands = {}
    for v in arr:
        if v in cands:
            cands[v] += 1
        elif len(cands) == k - 1:
            removed = []
            for key, value in cands.items():
                if value == 1:
                    removed.append(key)
                else:
                    cands[key] -= 1
            for key in removed:
                del cands[key]
        else:
            cands[v] = 1
    reals = {}
    for v in arr:
        if v in cands:
            if v in reals:
                reals[v] += 1
            else:
                reals[v] = 1
    has_print = False
    for key, c in reals.items():
        if c > len(arr) / k:
            has_print = True
            print(key, end=' ')
    if not has_print:
        print('no such number')
    else:
        print()


if __name__ == '__main__':
    n2([1, 2, 2, 1, 1])
    n2([1, 2, 2, 1])
    n2([1, 2, 3])
    nk([1, 2, 2, 1, 1], 2)
    nk([1, 2, 2, 1], 2)
    nk([1, 2, 3], 2)
