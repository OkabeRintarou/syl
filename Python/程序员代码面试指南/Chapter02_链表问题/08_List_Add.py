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


def reverse_list(head):
    if not head or not head.next:
        return head

    prev, cur = None, head
    while cur:
        n = cur.next
        cur.next = prev
        prev = cur
        cur = n
    return prev


def list_add(head1, head2):
    head1, head2 = reverse_list(head1), reverse_list(head2)
    carry = 0
    data = []
    while head1 and head2:
        s = head1.value + head2.value + carry
        carry = s // 10
        data.append(s % 10)
        head1 = head1.next
        head2 = head2.next
    if not head1 and not head2 and carry:
        data.append(carry)
    elif head1:
        while head1:
            s = head1.value + carry
            carry = s // 10
            data.append(s % 10)
            head1 = head1.next
        if carry:
            data.append(carry)
    else:
        while head2:
            s = head2.value + carry
            carry = s // 10
            data.append(s % 10)
            head2 = head2.next
        if carry:
            data.append(carry)
    for v in data[::-1]:
        print(v, end='')


if __name__ == '__main__':
    list_add(build_list((9, 3, 7)), build_list((6, 3)))
