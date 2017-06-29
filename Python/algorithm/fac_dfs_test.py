import unittest
import sys

from fac_dfs import fac

def myfac(n):
    rc = 1
    for i in range(2,n + 1):
        rc *= i
    return rc

class TestFacDFS(unittest.TestCase):
    def test_fac(self):
        for i in range(1,100):
            self.assertEqual(myfac(i),fac(i))

if __name__ == '__main__':
    sys.setrecursionlimit(sys.getrecursionlimit()  * 10)
    unittest.main()
