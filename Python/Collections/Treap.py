#!/usr/bin/python3
# -*- coding:utf-8 -*-

from random import random


class _Node(object):
    def __init__(self, key, val):
        self.weight = random()
        self.lhs = self.rhs = None
        self.key = key
        self.val = val


class Treap(object):
    def __init__(self):
        self._root = None

    @staticmethod
    def _right_rotate(cur_node, lhs_node):
        cur_node.lhs = lhs_node.rhs
        lhs_node.rhs = cur_node
        return lhs_node

    @staticmethod
    def _left_rotate(cur_node, rhs_node):
        cur_node.rhs = rhs_node.lhs
        rhs_node.lhs = cur_node
        return rhs_node

    def _insert(self, cur_node, ins_node):
        if cur_node is None:
            return ins_node
        elif ins_node.key == cur_node.key:
            cur_node.val = ins_node.val
            return cur_node
        elif ins_node.key < cur_node.key:
            cur_node.lhs = self._insert(cur_node.lhs, ins_node)
            if cur_node.lhs.weight < cur_node.weight:
                cur_node = Treap._right_rotate(cur_node, cur_node.lhs)
        else:
            cur_node.rhs = self._insert(cur_node.rhs, ins_node)
            if cur_node.rhs.weight < cur_node.weight:
                cur_node = Treap._left_rotate(cur_node, cur_node.rhs)
        return cur_node

    def is_empty(self):
        return self._root is None

    def put(self, key, value):
        new_node = _Node(key, value)
        self._root = self._insert(self._root, new_node)

    def get(self, key):
        cur = self._root
        while cur:
            if key < cur.key:
                cur = cur.lhs
            elif cur.key < key:
                cur = cur.rhs
            else:
                return cur.value
        return None

    def _update_parent(self, parent, left, new_node):
        if parent:
            if left:
                parent.lhs = new_node
            else:
                parent.rhs = new_node
        else:
            self._root = new_node

    def _delete_node(self, cur, parent, left):
        if cur.lhs and cur.rhs:
            if cur.lhs.weight < cur.rhs.weight:
                parent, left = Treap._right_rotate(cur, cur.lhs), False
                Treap._delete_node(cur, parent, left)
            else:
                parent, left = Treap._left_rotate(cur, cur.rhs), True
                Treap._delete_node(cur, parent, left)
        else:
            self._update_parent(parent, left, cur.lhs if cur.lhs else cur.rhs)

    def delete(self, key):
        parent = None
        left = False
        cur = self._root
        while cur and cur.key != key:
            parent = cur
            if cur.key < key:
                cur = cur.rhs
                left = False
            else:
                cur = cur.lhs
                left = True
        if cur:
            if not parent and not cur.lhs and not cur.rhs:
                self._root = None
            else:
                self._delete_node(cur, parent, left)
            return True
        return False

    def min(self):
        cur = self._root
        while cur and cur.lhs:
            cur = cur.lhs
        if cur:
            return cur.val
        return None

    def max(self):
        cur = self._root
        while cur and cur.rhs:
            cur = cur.rhs
        if cur:
            return cur.val
        return None
