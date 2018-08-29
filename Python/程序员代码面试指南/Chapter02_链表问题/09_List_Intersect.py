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


def get_loop_node(head):
    if not head or not head.next or not head.next.next:
        return None
    slow = head.next
    fast = head.next.next
    while slow != fast:
        if not fast.next or not fast.next.next:
            return None
        slow = slow.next
        fast = fast.next.next
    fast = head
    while fast != slow:
        fast = fast.next
        slow = slow.next
    return slow


def list_tail(head):
    if not head or not head.next:
        return head

    cur = head.next
    while cur.next:
        cur = cur.next
    return cur


def no_loop(head1, head2, stop1, stop2):
    if not head1 or not head2:
        return None
    if head1 == head2:
        return head1

    cur1, cur2 = head1.next, head2.next
    len1, len2 = 1, 1
    while cur1.next != stop1:
        len1 += 1
        cur1 = cur1.next
    while cur2.next != stop2:
        len2 += 1
        cur2 = cur2.next

    if cur1 != cur2:
        return None
    cur1, cur2 = head1, head2
    if len1 > len2:
        for i in range(len1 - len2):
            cur1 = cur1.next
    elif len1 < len2:
        for i in range(len2 - len1):
            cur2 = cur2.next

    while cur1 != cur2:
        cur1 = cur1.next
        cur2 = cur2.next
    return cur1


def loop(head1, head2, loop1, loop2):
    if loop1 == loop2:
        return no_loop(head1, head2, loop1, loop2)
    cur = loop1.next
    while cur != loop1:
        if cur == loop2:
            return cur
        cur = cur.next
    return None


def list_intersect(head1, head2):
    if not head1 or not head2:
        return None
    loop1, loop2 = get_loop_node(head1), get_loop_node(head2)
    if not loop1 and not loop2:
        return no_loop(head1, head2, None, None)
    elif loop1 and loop2:
        return loop(head1, head2, loop1, loop2)
    return None


if __name__ == '__main__':
    head1 = build_list([_ for _ in range(100)])
    head2 = build_list([_ * 100 for _ in range(50)])
    tail1 = list_tail(head1)
    tail2 = list_tail(head2)
    head3 = build_list([_ for _ in range(20)])
    tail1.next = head3
    tail2.next = head3
    common = list_intersect(head1, head2)
    print(common.value)
