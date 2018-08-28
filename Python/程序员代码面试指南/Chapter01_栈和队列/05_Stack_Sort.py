#!/usr/bin/python3
# -*- coding:utf-8 -*-
import random
from collections import deque


class Stack(object):
    def __init__(self):
        self.data = deque()

    def push(self, value):
        self.data.append(value)

    def pop(self):
        return self.data.pop()

    def top(self):
        return self.data[-1]

    def is_empty(self):
        return len(self.data) == 0


def sort_stack(s):
    help_stk = Stack()
    while not s.is_empty():
        v = s.pop()
        if help_stk.is_empty():
            help_stk.push(v)
        elif v <= help_stk.top():
            help_stk.push(v)
        else:
            while not help_stk.is_empty():
                s.push(help_stk.pop())
            help_stk.push(v)

    while not help_stk.is_empty():
        s.push(help_stk.pop())


if __name__ == '__main__':
    arr = [i for i in range(10)]
    random.shuffle(arr)
    stk = Stack()
    for i in arr:
        stk.push(i)
        print(i, end=' ')
    print()
    sort_stack(stk)
    while not stk.is_empty():
        print(stk.pop(), end=' ')
    print()
