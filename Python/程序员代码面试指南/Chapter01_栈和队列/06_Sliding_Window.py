#!/usr/bin/python3
# -*- coding:utf-8 -*-
from collections import deque


class SlidingWindow(object):
    def __init__(self, w, arr):
        self.q = deque()
        self.w = w
        self.arr = arr

    def put(self, index):
        if len(self.q) == 0:
            self.q.append(index)
            return
        last = self.q[-1]
        if self.arr[index] < self.arr[last]:
            self.q.append(index)
        else:
            self.q.pop()
            while len(self.q) > 0 and self.q[0] < index - self.w + 1:
                self.q.popleft()
            self.put(index)

    def max(self):
        return self.arr[self.q[0]]


if __name__ == '__main__':
    arr = [4, 3, 5, 4, 3, 3, 6, 7]
    w = 3
    sw = SlidingWindow(w, arr)
    for i in range(w):
        sw.put(i)
    print(sw.max())
    for i in range(w, len(arr)):
        sw.put(i)
        print(sw.max())
