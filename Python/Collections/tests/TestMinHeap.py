#!/usr/bin/python3
# -*- coding:utf-8 -*-
import unittest
import random
import heapq
from MinHeap import MinHeap


class TestMinHeap(unittest.TestCase):
    def test_min1(self):
        heap1 = MinHeap()
        data = [5, 1, 2, 3, 4]
        for v in data:
            heap1.push(v)
        self.assertEqual(1, heap1.min())

    def test_min(self):
        heap = MinHeap()
        keys = random.sample(range(10000), 5000)
        min_key = min(keys)
        for key in keys:
            heap.push(key)
        self.assertEqual(min_key, heap.min())
        heapq.heapify(keys)
        while len(keys) > 0:
            self.assertFalse(heap.is_empty())
            self.assertEqual(heapq.heappop(keys), heap.min())
            heap.pop()
        self.assertTrue(heap.is_empty())
