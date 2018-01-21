#!/usr/bin/python3
# -*- coding:utf-8 -*-
import unittest
import random
import heapq

from Treap import Treap


class TestTreap(unittest.TestCase):
    def test_put_delete_0(self):
        tree = Treap()
        tree.put(1, 1)
        tree.put(3, 3)
        tree.put(2, 2)
        tree.put(5, 5)
        tree.put(4, 4)
        self.assertEqual(1, tree.min())
        self.assertEqual(5, tree.max())
        tree.delete(1)
        self.assertEqual(2, tree.min())
        self.assertEqual(5, tree.max())
        tree.delete(5)
        self.assertEqual(2, tree.min())
        self.assertEqual(4, tree.max())
        tree.delete(3)
        self.assertEqual(2, tree.min())
        self.assertEqual(4, tree.max())
        tree.delete(4)
        self.assertEqual(2, tree.min())
        self.assertEqual(2, tree.max())
        tree.delete(2)
        self.assertTrue(tree.is_empty())

    def test_put_delete(self):
        for _ in range(10):
            tree = Treap()
            keys = random.sample(range(100000), random.randint(50000, 100000))
            for key in keys:
                tree.put(key, '{}'.format(key))
            heapq.heapify(keys)
            while len(keys) > 0:
                self.assertFalse(tree.is_empty())
                min_key = heapq.heappop(keys)
                self.assertEqual('{}'.format(min_key), tree.min())
                tree.delete(min_key)


if __name__ == '__main__':
    unittest.main()
