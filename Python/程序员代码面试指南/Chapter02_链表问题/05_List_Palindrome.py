#!/usr/bin/python3
# -*- coding:utf-8 -*-
from collections import deque


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


def is_palindrome1(head):
    if head is None:
        return True
    stk = deque()
    h = head
    while h:
        stk.append(h.value)
        h = h.next
    h = head
    while h:
        if h.value != stk.pop():
            return False
        h = h.next
    return True


def is_palindrome2(head):
    if not head or not head.next:
        return True
    right = head.next
    cur = head
    while cur.next and cur.next.next:
        right = right.next
        cur = cur.next.next

    stk = deque()
    while right:
        stk.append(right.value)
        right = right.next
    while len(stk) > 0:
        if stk.pop() != head.value:
            return False
        head = head.next
    return True


if __name__ == '__main__':
    test1 = build_list((1, 2, 3, 4))
    test2 = build_list((1, 2, 3, 2, 1))
    test3 = build_list((1, 2, 2, 1))
    assert not is_palindrome1(test1)
    assert is_palindrome1(test2)
    assert is_palindrome1(test3)

    assert not is_palindrome2(test1)
    assert is_palindrome2(test2)
    assert is_palindrome2(test3)
