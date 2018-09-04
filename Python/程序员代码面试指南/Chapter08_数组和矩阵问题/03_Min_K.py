#!/usr/bin/python3
# -*- coding:utf-8 -*-


def find_minimum_k(arr, k):
    if k < 1 or k >= len(arr):
        return arr
    heap = []
    for i in range(k):
        heap_insert(heap, arr[i])
    heap.append(0)
    for i in arr[k:]:
        if i < heap[0]:
            heapify(heap, i)
    return heap[:k]


def heapify(hp, v):
    hp[0] = v
    cur = 0

    while True:
        lhs, rhs = 2 * (cur + 1) - 1, 2 * (cur + 1)
        n = len(hp)
        if lhs < len(hp) and hp[lhs] > v:
            n = lhs
        if rhs < len(hp) and hp[rhs] > hp[lhs]:
            n = rhs
        if not n < len(hp):
            break
        hp[n], hp[cur] = hp[cur], hp[n]
        cur = n


def heap_insert(hp, v):
    index = len(hp)
    hp.append(v)
    parent = (index - 1) // 2
    while parent >= 0 and hp[index] > hp[parent]:
        hp[index], hp[parent] = hp[parent], hp[index]
        index = parent
        parent = (parent - 1) // 2


if __name__ == '__main__':
    print(find_minimum_k([5, 4, 3, 7, 6, 8, 0, 9, 1, 2], 4))
