#!/usr/bin/python3
# -*- coding:utf-8 -*-
from common import tree


def print_node(node):
    print(node.value, end=' ')


def print_boundary(node):
    if not node:
        return
    height = node.height()
    levels = [[None, None] for _ in range(height + 1)]
    set_edge_map(node, 0, levels)
    for i in range(height + 1):
        print_node(levels[i][0])
    print_leaf(node, 0, levels)
    for i in range(height + 1):
        n = levels[height - i][1]
        if n != levels[height - i][0]:
            print_node(n)


def print_leaf(node, h, levels):
    if not node:
        return
    if h == len(levels) - 1:
        if node != levels[h][0] and node != levels[h][1]:
            print_node(node)
    else:
        print_leaf(node.left, h + 1, levels)
        print_leaf(node.right, h + 1, levels)


def set_edge_map(node, h, levels):
    if not node:
        return
    if not levels[h][0]:
        levels[h][0] = node
    levels[h][1] = node
    set_edge_map(node.left, h + 1, levels)
    set_edge_map(node.right, h + 1, levels)


if __name__ == '__main__':
    t = tree.build_tree(
        [1, 2, 3, None, 4, 5, 6, 7, 8, 9, 10, None, None, None, None, None, 11, 12,
         None, None, None, 13, 14, 15, 16, None, None, None, None, None, None, None, None])
    print_boundary(t)
