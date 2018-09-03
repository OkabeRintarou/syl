#!/usr/bin/python3
# -*- coding:utf-8 -*-
import collections
from common.tree import *


def pre_order_recur(node):
    if not node:
        return
    print(str(node.value), end=' ')
    pre_order_recur(node.left)
    pre_order_recur(node.right)


def pre_order_unrecur(node):
    if not node:
        return
    s = collections.deque()
    s.append(node)

    while len(s):
        cur = s.pop()
        print(str(cur.value), end=' ')
        if cur.right:
            s.append(cur.right)
        if cur.left:
            s.append(cur.left)


def in_order_recur(node):
    if not node:
        return
    in_order_recur(node.left)
    print(str(node.value), end=' ')
    in_order_recur(node.right)


def in_order_unrecur(node):
    if not node:
        return
    s = collections.deque()
    s.append(node)
    while node.left:
        s.append(node.left)
        node = node.left
    while len(s):
        cur = s.pop()
        print(str(cur.value), end=' ')
        if cur.right:
            s.append(cur.right)
            cur = cur.right
            while cur.left:
                s.append(cur.left)
                cur = cur.left


def post_order_recur(node):
    if not node:
        return
    post_order_recur(node.left)
    post_order_recur(node.right)
    print(str(node.value), end=' ')


def post_order_unrecur1(node):
    if not node:
        return
    s1, s2 = collections.deque(), collections.deque()
    s1.append(node)
    while len(s1):
        cur = s1.pop()
        s2.append(cur)
        if cur.left:
            s1.append(cur.left)
        if cur.right:
            s1.append(cur.right)
    while len(s2):
        print(str(s2.pop().value), end=' ')


def post_order_unrecur2(node):
    if not node:
        return
    s = collections.deque()
    s.append(node)
    p = None
    while len(s):
        cur = s[-1]
        if cur.left and p != cur.left and p != cur.right:
            s.append(cur.left)
        elif cur.right and p != cur.right:
            s.append(cur.right)
        else:
            print(str(cur.value), end=' ')
            p = s.pop()


if __name__ == '__main__':
    t = build_tree([1, 2, 3, 4, 5, 6, 7, None, None, None, None, None, None, None, None])
    pre_order_recur(t)
    print()
    pre_order_unrecur(t)
    print()
    in_order_recur(t)
    print()
    in_order_unrecur(t)
    print()
    post_order_recur(t)
    print()
    post_order_unrecur1(t)
    print()
    post_order_unrecur2(t)
