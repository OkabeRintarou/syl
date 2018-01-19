#!/usr/bin/python3
# -*- coding:utf-8 -*-


def _parent(index):
    return (index - 1) >> 1


def _left_child(index):
    return (index << 1) + 1


def _right_child(index):
    return (index << 1) + 2


class MinHeap(object):
    def __init__(self):
        self._data = []

    def min(self):
        if len(self._data) > 0:
            return self._data[0]
        raise IndexError

    def is_empty(self):
        return len(self._data) == 0

    def _swap(self, lhs, rhs):
        self._data[lhs], self._data[rhs] = self._data[rhs], self._data[lhs]

    def _up(self, index):
        cur, p = index, _parent(index)
        while p >= 0 and self._data[p] > self._data[cur]:
            self._swap(p, cur)
            cur = p
            p = _parent(cur)

    def push(self, val):
        self._data.append(val)
        self._up(len(self._data) - 1)

    def _min_index(self, left, right, cur):
        min_index, upper = cur, len(self._data)
        if left < upper and self._data[left] < self._data[cur]:
            min_index = left
        if right < upper and self._data[right] < self._data[min_index]:
            min_index = right
        return min_index

    def _down(self, index):
        left_child, right_child = _left_child(index), _right_child(index)
        swap_index = self._min_index(left_child, right_child, index)
        if swap_index != index:
            self._swap(index, swap_index)
            self._down(swap_index)

    def pop(self):
        if self.is_empty():
            raise IndexError
        self._swap(0, len(self._data) - 1)
        self._data.pop()
        if not self.is_empty():
            self._down(0)
