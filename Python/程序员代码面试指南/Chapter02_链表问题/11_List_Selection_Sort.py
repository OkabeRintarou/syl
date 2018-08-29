#!/usr/bin/python3
# -*- coding:utf-8 -*-
import random


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


def get_smallest_prev(head):
    if not head:
        return None
    prev = None
    smallest = head.value
    while head.next:
        if smallest > head.next.value:
            prev = head
            smallest = head.next.value
        head = head.next
    return prev


def list_sort(head):
    if not head or not head.next:
        return head
    new_head, tail = None, None
    while head:
        prev = get_smallest_prev(head)
        if not prev:
            # head is smallest
            cur = head
            head = head.next
            if not new_head:
                new_head = cur
                tail = new_head
            else:
                tail.next = cur
                tail = cur
            cur.next = None
        else:
            cur = prev.next
            if not new_head:
                new_head = cur
                tail = new_head
            else:
                tail.next = cur
                tail = cur
            prev.next = cur.next
            cur.next = None
    return new_head


def is_sorted(head):
    if not head or not head.next:
        return True
    prev, cur = head, head.next
    while cur:
        if prev.value > cur.value:
            return False
        prev = cur
        cur = cur.next
    return True


if __name__ == '__main__':
    data = [_ for _ in range(100)]
    random.shuffle(data)
    assert is_sorted(list_sort(build_list(data)))
