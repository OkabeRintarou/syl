#!/usr/bin/python3
# -*- coding:utf-8 -*-


def swap(a, b):
    a = a ^ b
    b = a ^ b
    a = a ^ b
    return a, b


if __name__ == '__main__':
    a, b = 100, 200
    print(swap(a, b))
