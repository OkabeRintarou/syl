#!/usr/bin/python3
# -*- coding:utf-8 -*-

def _gen_graph(arr):
    return {elem:[x for x in arr if x != elem] for elem in arr}


def dfs(graph,src):
    rc = []
    path = []
    visited = {}
    nodes = tuple(graph.keys())
    def _dfs_impl(src):
        visited[src] = True
        path.append(src)
        if len(visited) == len(nodes):
            rc.append(tuple(path))
        for u in graph[src]:
            if u not in visited:
                _dfs_impl(u)
        del visited[src]
        path.pop()

    _dfs_impl(src)
    return rc


def permutation(arr):
    graph = _gen_graph(arr)
    nodes = tuple(graph.keys())
    rc = []
    for node in nodes:
        rc.extend(dfs(graph,node))
    
    return rc
