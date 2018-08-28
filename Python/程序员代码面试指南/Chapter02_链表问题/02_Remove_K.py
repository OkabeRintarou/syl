#!/usr/bin/python3
# -*- coding:utf-8 -*-


class SNode(object):
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


def build_single_list(data):
    if len(data) == 0:
        return None
    if len(data) == 1:
        return SNode(data[0])
    tail = build_single_list(data[1:])
    node = SNode(data[0])
    node.next = tail
    return node


def single_remove_k(head, k):
    if not head or k < 1:
        return
    v = k
    n = head
    while n:
        v -= 1
        if k == 1 and n.next and not n.next.next:
            n.next = None
            return head
        n = n.next
    if v == 0:
        head = head.next
    elif v < 0:
        v = k
        n = head
        while n:
            v -= 1
            if v == 1:
                n.next = n.next.next
                break
            n = n.next
    return head


if __name__ == '__main__':
    arr = [1, 3, 2]
    for i in range(1, 5):
        l = build_single_list(arr)
        l = single_remove_k(l, i)
        print(i, ':', l)
