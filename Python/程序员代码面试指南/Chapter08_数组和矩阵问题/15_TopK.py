#!/usr/bin/python3
# -*- coding:utf-8 -*-


def p(i):
    return (i - 1) // 2


def lc(i):
    return (i + 1) * 2 - 1


def rc(i):
    return (i + 1) * 2


def heap_insert(h, v):
    h.append(v)
    i = len(h) - 1
    while i > 0:
        pt = p(i)
        if h[pt][0] < h[i][0]:
            h[pt], h[i] = h[i], h[pt]
            i = pt
        else:
            break


def heapify(h, t, arr):
    h[0] = h[-1]
    h.pop()
    i = 0
    while i < len(h):
        l, r, n = lc(i), rc(i), i
        if r < len(h):
            if h[i][0] > h[l][0] and h[i][0] > h[r][0]:
                break
            elif h[l][0] > h[r][0]:
                h[l], h[i] = h[i], h[l]
                i = l
            else:
                h[r], h[i] = h[i], h[r]
                i = r
        elif l < len(h):
            if h[i][0] > h[l][0]:
                break
            else:
                h[i], h[l] = h[l], h[i]
                i = l
        else:
            break
    if t[2] > 0:
        heap_insert(h, (arr[t[1]][t[2] - 1], t[1], t[2] - 1))


def top_k(arr, k):
    heap = []
    for i in range(len(arr)):
        if arr[i]:
            heap_insert(heap, (arr[i][-1], i, len(arr[i]) - 1))
    i = 0
    while i < k:
        t = heap[0]
        print(t[0], end='\n')
        heapify(heap, t, arr)
        i += 1


if __name__ == '__main__':
    top_k([[219, 405, 538, 845, 971], [148, 558], [52, 99, 348, 691]], 5)
