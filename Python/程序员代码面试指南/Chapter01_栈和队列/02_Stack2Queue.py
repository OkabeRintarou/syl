#!/usr/bin/python3
# -*- coding:utf-8 -*-

class Stack(object):
	def __init__(self):
		self.data = []

	def push(self,v):
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

class Queue(object):
	def __init__(self):
		self.main_stack = Stack()
		self.backup_stack = Stack()

	def add(self,v):
		self.main_stack.push(v)

	def poll(self):
		if not self.backup_stack.is_empty():
			return self.backup_stack.pop()
		if self.main_stack.is_empty():
			raise Exception('empty queue')
		while not self.main_stack.is_empty():
			self.backup_stack.push(self.main_stack.pop())
		return self.backup_stack.pop()

	def peek(self):
		if not self.backup_stack.is_empty():
			return self.backup_stack.top()
		if self.main_stack.is_empty():
			raise Exception('empty queue')
		while not self.main_stack.is_empty():
			self.backup_stack.push(self.main_stack.pop())
		return self.backup_stack.top()

	def is_empty(self):
		return len(self.main_stack) == 0 and len(self.backup_stack) == 0

if __name__ == '__main__':
	def testBasic():
		q = Queue()
		for i in range(100):
			q.add(i)
		for i in range(100):
			assert(i == q.poll())
	testBasic()
