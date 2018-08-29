#!/usr/bin/python3
# -*- coding:utf-8 -*-


class Node(object):
    def __init__(self, v):
        self.value = v
        self.next = None
        self.random = None

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


def random_copy(head):
    if not head:
        return None
    prev, cur = None, head
    while cur:
        if prev:
            prev.next = cur
        n = cur.next
        c = Node(cur.value)
        cur.next = c
        prev = c
        cur = n

    cur, n = head, head.next
    while cur:
        cur_next = n.next
        n_next = None
        if cur_next:
            n_next = cur_next.next

        if cur.random:
            n.random = cur.random.next
        n.next = n_next
        cur = cur_next
        n = n_next
    return head.next


if __name__ == '__main__':
    node1 = Node(1)
    node2 = Node(2)
    node3 = Node(3)
    node1.next = node2
    node1.random = node3
    node2.next = node3
    node3.random = node1

    print(random_copy(node1))
