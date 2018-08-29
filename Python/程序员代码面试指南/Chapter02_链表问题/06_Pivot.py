#!/usr/bin/python3
# -*- coding:utf-8 -*-


class Node(object):
    def __init__(self, v):
        self.value = v
        self.next = None

    def __repr__(self):
        arr = [str(self.value)]
        n = self.next
        while n:
            arr.append(str(n.value))
            n = n.next
        return ' '.join(arr)


def build_list(data):
    if len(data) == 0:
        return None
    if len(data) == 1:
        return Node(data[0])
    tail = build_list(data[1:])
    node = Node(data[0])
    node.next = tail
    return node


def pivot(head, p):
    h1, h2, h3 = None, None, None
    t1, t2, t3 = None, None, None

    def list_append(h, t, n):
        assert n
        n.next = h
        if not h:
            return n, n
        return n, t

    while head:
        n = head.next
        if head.value < p:
            h1, t1 = list_append(h1, t1, head)
        elif head.value == p:
            h2, t2 = list_append(h2, t2, head)
        else:
            h3, t3 = list_append(h3, t3, head)
        head = n

    if h1:
        head = h1
    if h2:
        if not head:
            head = h2
        else:
            t1.next = h2

    if h3:
        if not head:
            head = h3
        elif h2:
            t2.next = h3
        elif t1:
            t1.next = h3

    return head


if __name__ == '__main__':
    print(pivot(build_list((9, 0, 4, 5, 1)), 3))
    print(pivot(build_list((7, 9, 1, 8, 5, 2, 5)), 5))
