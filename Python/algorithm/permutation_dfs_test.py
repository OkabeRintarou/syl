#!/usr/bin/python3
# -*- coding:utf-8 -*-

import unittest
from itertools import permutations
from permutation_dfs import permutation

class TestPermutation(unittest.TestCase):
    def test_permutation(self):
        for num in range(1,10):
            arr = [x for x in range(num)]
            self.assertEqual(set(permutations(arr)),set(permutation(arr)))

if __name__ == '__main__':
	unittest.main()
