#!/usr/bin/python3
# -*- coding:utf-8 -*-

def _dfs(n):
    visited = {}
    def _dfs_impl(n):
        visited[n] = True
        if n == 2:
            return 2
        elif n == 1:
            return 1
        if n - 1 not in visited:
            return n * _dfs_impl(n - 1)
    return _dfs_impl(n)

def fac(n):
    return _dfs(n)


