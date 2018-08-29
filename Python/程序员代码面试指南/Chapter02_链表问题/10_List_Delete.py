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


def list_delete(head, num):
    cur = head
    while cur and cur.value == num:
        cur = cur.next
    new_head = cur
    if cur:
        prev = cur
        cur = cur.next
        while cur:
            if cur.value == num:
                prev.next = cur.next
            else:
                prev = cur
            cur = cur.next

    return new_head


if __name__ == '__main__':
    print(list_delete(build_list([3 for _ in range(100)]), 3))
    print(list_delete(build_list([_ for _ in range(10)]), 3))
    print(list_delete(build_list([3, 3, 1, 2, 3, 4, 3, 3, 6, 7, 3, 3]), 3))
