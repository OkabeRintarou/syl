#!/usr/bin/python3
# -*- coding:utf-8 -*-
import collections


def shortest_path(arr):
    if not arr or not arr[0]:
        return 0
    mp = []
    for i in range(len(arr)):
        mp.append([])
        for j in range(len(arr[i])):
            mp[-1].append(0)
    q = collections.deque()
    row, col = len(arr), len(arr[0])

    def walk_to(prevx, prevy, x, y):
        # can move to (x,y)
        if 0 <= x < row and 0 <= y < col and arr[x][y] and not mp[x][y]:
            q.append((x, y))
            mp[x][y] = mp[prevx][prevy] + 1

    q.append((0, 0))
    mp[0][0] = 1
    while q:
        cur = q.popleft()
        if cur[0] == row - 1 and cur[1] == col - 1:
            return mp[cur[0]][cur[1]]

        walk_to(cur[0], cur[1], cur[0] - 1, cur[1])  # up
        walk_to(cur[0], cur[1], cur[0] + 1, cur[1])  # down
        walk_to(cur[0], cur[1], cur[0], cur[1] + 1)  # right
        walk_to(cur[0], cur[1], cur[0], cur[1] - 1)  # left


if __name__ == '__main__':
    print(shortest_path([[1, 0, 1, 1, 1], [1, 0, 1, 0, 1], [1, 1, 1, 0, 1], [0, 0, 0, 0, 1]]))
