#!/usr/bin/python3
# -*- coding:utf-8 -*-

import heapq


class Median(object):
    def __init__(self):
        self.min_heap = []
        self.max_heap = []

    def put(self, value):
        if len(self.min_heap) == len(self.max_heap):
            if len(self.min_heap) != 0:
                heapq.heappush(self.max_heap, -heapq.heappop(self.min_heap))
            heapq.heappush(self.min_heap, value)
        elif len(self.min_heap) > len(self.max_heap):
            heapq.heappush(self.max_heap, -value)
        else:
            heapq.heappush(self.min_heap, value)

    def median(self):
        if len(self.min_heap) > len(self.max_heap):
            return self.min_heap[0]
        elif len(self.min_heap) < len(self.max_heap):
            return -self.max_heap[0]
        else:
            return (self.min_heap[0] - self.max_heap[0]) / 2.0


if __name__ == '__main__':
    m = Median()
    for i in range(1, 11):
        m.put(i)
        print(i, m.median())
