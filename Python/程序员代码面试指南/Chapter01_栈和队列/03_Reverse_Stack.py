#!/usr/bin/python3
# -*- coding:utf-8 -*-


class Stack(object):
    def __init__(self):
        self.data = []

    def push(self, v):
        self.data.append(v)

    def pop(self):
        if len(self.data) == 0:
            raise Exception('empty stack')
        return self.data.pop()

    def top(self):
        if len(self.data) == 0:
            raise Exception('empty stack')
        return self.data[-1]

    def is_empty(self):
        return len(self.data) == 0

    def __repr__(self):
        d = ['[']
        for v in self.data[::-1]:
            d.append(str(v))
        d.append(']')
        return ' '.join(d)


def get_and_remove_last(s):
    result = s.pop()
    if s.is_empty():
        return result
    last = get_and_remove_last(s)
    s.push(result)
    return last


def reverse(s):
    if s.is_empty():
        return
    last = get_and_remove_last(s)
    reverse(s)
    s.push(last)


if __name__ == '__main__':
    stk = Stack()
    for i in range(1, 6):
        stk.push(i)

    print('before: ', stk)
    reverse(stk)
    print('after: ', stk)
