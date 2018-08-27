#!/usr/bin/python3
# -*- coding:utf-8 -*-


def _combination(res, ch):
    arr = []
    for v in res:
        for i in range(len(v) + 1):
            lst = list(v)
            lst.insert(i, ch)
            arr.append(''.join(lst))
    return res + arr


def combination(data):
    if len(data) == 0:
        return []

    res = [data[0]]
    for ch in data[1:]:
        res = _combination(res, ch)
    return res


if __name__ == '__main__':
    print(combination('a'))
    print(combination('ab'))
    print(combination('abc'))
