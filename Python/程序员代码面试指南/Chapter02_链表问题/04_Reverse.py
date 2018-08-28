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


def reverse_aux(head):
    if not head or not head.next:
        return head, head
    n, tail = reverse_aux(head.next)
    tail.next = head
    head.next = None
    return n, head


def reverse(head):
    head, _ = reverse_aux(head)
    return head


if __name__ == '__main__':
    for i in range(8):
        print(reverse(build_list([_ for _ in range(i)])))
