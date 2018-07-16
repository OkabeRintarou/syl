import unittest
import random
from RedBlackTree import RedBlackTree


class TestRedBlackTree(unittest.TestCase):
    def test_basic(self):
        tree = RedBlackTree()
        for i in range(1, 64):
            self.assertIsNone(tree.get(i))
            tree.put(i, i)
            self.assertEqual(i, tree.get(i))
            tree.delete(i)
            self.assertIsNone(tree.get(i))

        self.assertTrue(tree.is_empty())

    def test_insert(self):
        tree = RedBlackTree()
        keys = set()
        total = 100000
        for i in range(total):
            key = random.random()
            keys.add(key)
            tree.put(key, key)
        self.assertEqual(len(keys), tree.size())

    def test_random(self):
        tree = RedBlackTree()
        mp = {}

        def insert(k, v):
            mp[k] = v
            tree.put(k, v)

        def delete(k):
            del mp[k]
            tree.delete(k)

        keys = []
        total = 200000
        for i in range(total):
            k, v = random.random(), random.random()
            if random.randint(0, 2) > 0:
                # insert
                insert(k, v)
                keys.append(k)
            elif len(keys) > 0:
                idx = random.randint(0, len(keys) - 1)
                k = keys[idx]
                keys = keys[:idx] + keys[idx + 1:]
                delete(k)

        for k, v in mp.items():
            value = tree.get(k)
            self.assertIsNotNone(value)
            self.assertEqual(v, value)


if __name__ == '__main__':
    unittest.main()
