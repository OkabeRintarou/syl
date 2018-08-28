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


def remove_middle(head):
    if not head or not head.next:
        return head
    if not head.next.next:
        return head.next

    prev, cur = head, head.next.next

    while cur.next and cur.next.next:
        prev = prev.next
        cur = cur.next.next

    prev.next = prev.next.next
    return head


if __name__ == '__main__':
    print(remove_middle(build_list([_ for _ in range(1, 8)])))
    print(remove_middle(build_list([_ for _ in range(1, 9)])))
    print(remove_middle(build_list([_ for _ in range(1, 10)])))
