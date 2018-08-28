#!/usr/bin/python3
# -*- coding:utf-8 -*-


class Stack(object):
    def __init__(self):
        self.stack_data = []
        self.stack_min = []

    def push(self, value):
        self.stack_data.append(value)
        if len(self.stack_min) == 0:
            self.stack_min.append(value)
        else:
            t = self.stack_min[-1]
            if value < t:
                self.stack_min.append(value)
            else:
                self.stack_min.append(t)

    def pop(self):
        if len(self.stack_data) == 0 or len(self.stack_min) == 0:
            raise Exception('empty stack')
        self.stack_min.pop()
        return self.stack_data.pop()

    def top(self):
        if len(self.data) == 0:
            raise Exception('empty stack')
        return self.stack_data[-1]

    def min(self):
        if len(self.stack_min) == 0:
            raise Exception('empty stack')
        return self.stack_min[-1]

    def is_empty(self):
        return len(self.stack_data) == 0


if __name__ == '__main__':
    def testBasic():
        s = Stack()
        a = [1, 2, 1, 5, 4, 3]
        for v in a:
            s.push(v)
        while not s.is_empty():
            assert (s.min() == 1)
            s.pop()


    testBasic()
