#!/usr/bin/python3
# -*- coding:utf-8 -*-

_black = 0
_red = 1


class _Node(object):

    def __init__(self, key=None, value=None, color=_red):
        self.key = key
        self.value = value
        self.color = color

        self.parent = None
        self.left = None
        self.right = None

    def __str__(self):
        return str(self.key)


_nil = _Node(None, None, _black)


class RedBlackTree(object):

    def __init__(self):
        self.root = _nil
        _nil.parent = _nil
        _nil.left = _nil
        _nil.right = _nil

    def put(self, key, value):
        node = _Node(key, value)
        x = _nil
        y = self.root

        while y != _nil:
            x = y
            if key < y.key:
                y = y.left
            elif y.key < key:
                y = y.right
            else:
                y.value = value
                return

        node.parent = x
        if x is _nil:
            self.root = node
        elif x.key < node.key:
            x.right = node
        else:
            x.left = node

        node.left = _nil
        node.right = _nil

        self._insert_fixup(node)

    def _right_rotate(self, y):
        if self.root is y:
            self.root = y.left
        x = y.left
        y.left = x.right
        if y.left != _nil:
            y.left.parent = y
        x.right = y
        x.parent = y.parent
        p = y.parent
        y.parent = x
        if p.left == y:
            p.left = x
        else:
            p.right = x

    def _left_rotate(self, y):
        if self.root is y:
            self.root = y.right

        x = y.right
        y.right = x.left
        if y.right != _nil:
            y.right.parent = y
        x.left = y
        x.parent = y.parent
        p = y.parent
        y.parent = x
        if p.left == y:
            p.left = x
        else:
            p.right = x

    def _insert_fixup(self, z):

        while z.parent.color == _red:
            if z.parent == z.parent.parent.left:
                y = z.parent.parent.right
                if y != _nil and y.color == _red:
                    z.parent.color = _black
                    y.color = _black
                    z.parent.parent.color = _red
                    z = z.parent.parent
                else:
                    if z == z.parent.right:
                        z = z.parent
                        self._left_rotate(z)
                    z.parent.color = _black
                    z.parent.parent.color = _red
                    self._right_rotate(z.parent.parent)
            else:
                y = z.parent.parent.left
                if y and y.color == _red:
                    z.parent.color = _black
                    y.color = _black
                    z.parent.parent.color = _red
                    z = z.parent.parent
                else:
                    if z == z.parent.left:
                        z = z.parent
                        self._right_rotate(z)
                    z.parent.color = _black
                    z.parent.parent.color = _red
                    self._left_rotate(z.parent.parent)
        self.root.color = _black

    def _transplant(self, u, v):
        if u.parent == _nil:
            self.root = v
        elif u == u.parent.left:
            u.parent.left = v
        else:
            u.parent.right = v
        v.parent = u.parent

    def delete(self, key):
        z = self.root
        while z != _nil:
            if key == z.key:
                break
            elif key < z.key:
                z = z.left
            else:
                z = z.right
        if z != _nil:
            y = z
            y_original_color = y.color
            if z.left == _nil:
                x = z.right
                self._transplant(z, z.right)
            elif z.right == _nil:
                x = z.left
                self._transplant(z, z.left)
            else:
                y = self._minimum(z.right)
                y_original_color = y.color
                x = y.right
                if y.parent == z:
                    x.parent = y
                else:
                    self._transplant(y, y.right)
                    y.right = z.right
                    y.right.parent = y
                self._transplant(z, y)
                y.left = z.left
                y.left.parent = y
                y.color = z.color

            if y_original_color == _black:
                self._delete_fixup(x)

    def _delete_fixup(self, x):
        while x != self.root and x.color == _black:
            if x == x.parent.left:
                w = x.parent.right
                if w.color == _red:
                    w.color = _black
                    x.parent.color = _red
                    self._left_rotate(x.parent)
                    w = x.parent.right
                if w.left.color == _black and w.right.color == _black:
                    w.color = _red
                    x = x.parent
                else:
                    if w.right.color == _black:
                        w.left.color = _black
                        w.color = _red
                        self._right_rotate(w)
                        w = x.parent.right

                    w.color = x.parent.color
                    x.parent.color = _black
                    w.right.color = _black
                    self._left_rotate(x.parent)
                    x = self.root
            else:
                w = x.parent.left
                if w.color == _red:
                    w.color = _black
                    x.parent.color = _red
                    self._right_rotate(x.parent)
                    w = x.parent.left
                if w.left.color == _black and w.right.color == _black:
                    w.color = _red
                    x = x.parent
                else:
                    if w.left.color == _black:
                        w.right.color = _black
                        w.color = _red
                        self._left_rotate(w)
                        w = x.parent.left

                    w.color = x.parent.color
                    x.parent.color = _black
                    w.left.color = _black
                    self._right_rotate(x.parent)
                    x = self.root

        x.color = _black

    @staticmethod
    def _minimum(node):
        assert node and node != _nil
        p = node
        while p.left != _nil:
            p = p.left
        return p

    def get(self, key):
        x = self.root
        while x != _nil:
            if x.key < key:
                x = x.right
            elif x.key > key:
                x = x.left
            else:
                return x.value
        return None

    def size(self):
        return self._size(self.root)

    def _size(self, node):
        if node == _nil:
            return 0
        s = 0
        if node.left != _nil:
            s += self._size(node.left)
        if node.right != _nil:
            s += self._size(node.right)
        return s + 1

    def is_empty(self):
        return self.root is _nil

    def _depth(self, node):
        if node == _nil or (node.left == _nil and node.right == _nil):
            return 0
        d = 0
        if node.left != _nil:
            d = max(d, self._depth(node.left))
        if node.right != _nil:
            d = max(d, self._depth(node.right))
        return d + 1

    def depth(self):
        return self._depth(self.root)
