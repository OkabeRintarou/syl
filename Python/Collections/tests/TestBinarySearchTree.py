import unittest
import random
from BinarySearchTree import BinarySearchTree


class TestBinarySearchTree(unittest.TestCase):
    def test_put(self):
        tree = BinarySearchTree()
        keys = random.sample(range(1000000), 10000)
        for key in keys:
            tree.put(key, "value{}".format(key))
        for key in keys:
            self.assertTrue(tree.get(key) == "value{}".format(key))

    def test_get(self):
        tree = BinarySearchTree()
        self.assertIsNone(tree.get(1), "empty tree")
        keys = random.sample(range(1000), 100)
        for key in keys:
            tree.put("key{}".format(key), "value{}".format(key))
        for _ in range(100):
            index = random.randint(0, len(keys) - 1)
            self.assertEqual("value{}".format(keys[index]), tree.get("key{}".format(keys[index])))

        for i in range(1001, 1100):
            self.assertIsNone(tree.get("key{}".format(i)))

        # tree is list
        tree = BinarySearchTree()
        for i in range(1000):
            tree.put(i, i * 100)
        for i in range(1000):
            self.assertEqual(i * 100, tree.get(i))

    def test_delete(self):
        tree = BinarySearchTree()
        self.assertIsNone(tree.delete(1), "empty tree")

        keys = random.sample(range(1000), 100)
        for key in keys:
            tree.put("key{}".format(key), "value{}".format(key))
        for key in keys:
            v = tree.delete("key{}".format(key))
            self.assertEqual("value{}".format(key), v)

        tree = BinarySearchTree()
        for i in range(512):
            tree.put(i, i * 100)
        for i in range(512):
            self.assertEqual(i * 100, tree.delete(i))

        tree = BinarySearchTree()
        for i in range(128):
            tree.put(i, i * 100)
        for i in reversed(range(128)):
            self.assertEqual(i * 100, tree.delete(i))
            for j in range(i):
                self.assertEqual(j * 100, tree.get(j))

        self.assertIsNone(tree.delete(0))
