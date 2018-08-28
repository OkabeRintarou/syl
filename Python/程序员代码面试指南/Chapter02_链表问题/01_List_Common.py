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


def print_common(head1, head2):
    while head1 and head2:
        if head1.value < head2.value:
            head1 = head1.next
        elif head2.value < head1.value:
            head2 = head2.next
        else:
            print('common:', head1.value)
            head1 = head1.next
            head2 = head2.next


if __name__ == '__main__':
    arr1 = [1, 2, 3, 4, 5, 6]
    arr2 = [3, 5, 7, 8]
    print_common(build_list(arr1), build_list(arr2))
