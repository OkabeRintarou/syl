class _Node(object):
    def __init__(self, key=None, value=None):
        self.key = key
        self.value = value
        self.left_child = None
        self.right_child = None
        self.parent = None

    def set_left_child(self, left_child):
        self.left_child = left_child
        if left_child is not None:
            self.left_child.parent = self

    def set_right_child(self, right_child):
        self.right_child = right_child
        if right_child is not None:
            self.right_child.parent = self

    def attach(self, child):
        assert child is not None
        if child.key < self.key:
            self.set_left_child(child)
        else:
            self.set_right_child(child)

    def delete_child(self, child):
        assert child.left_child is None and child.right_child is None
        if child is self.right_child:
            self.right_child = None
        elif child is self.left_child:
            self.left_child = None
        else:
            assert False

    def subsequent(self):
        assert self.right_child is not None
        target = self.right_child
        delete_right = True
        while target.left_child is not None:
            target = target.left_child
            delete_right = False
        if delete_right:
            target.parent.set_right_child(self.right_child)
        else:
            target.parent.set_left_child(target.right_child)
        return target

    def precursor(self):
        assert self.left_child is not None
        target = self.left_child
        delete_left = True
        while target.right_child is not None:
            target = target.right_child
            delete_left = False
        if delete_left:
            target.parent.set_left_child(self.left_child)
        else:
            target.parent.set_right_child(target.left_child)
        return target


class BinarySearchTree(object):
    def __init__(self):
        self.root = None

    def __str__(self):
        pass

    def put(self, key, value):
        insert_point = self._insert_point(key)
        if insert_point is None:
            self.root = _Node(key, value)
            r = self.root
        elif key < insert_point.key:
            r = _Node(key, value)
            insert_point.set_left_child(r)
        elif insert_point.key < key:
            r = _Node(key, value)
            insert_point.set_right_child(r)
        else:
            insert_point.value = value
            r = insert_point
        return r

    def get(self, key):
        node = self._get(key)
        if node is None:
            return None
        return node.value

    def delete(self, key):
        node = self._get(key)
        if node is not None:
            r = node.value
            if node.left_child is not None:
                pre = node.precursor()
                assert pre is not None
                node.key = pre.key
                node.value = pre.value
            elif node.right_child is not None:
                sub = node.subsequent()
                assert sub is not None
                node.key = sub.key
                node.value = sub.value
            elif node.parent is not None:
                node.parent.delete_child(node)
            else:
                self.root = None

            return r
        return None

    def _get(self, key):
        insert_point = self._insert_point(key)
        if insert_point is None or key < insert_point.key or insert_point.key < key:
            return None
        return insert_point

    def _insert_point(self, key) -> (_Node, None):
        if self.root is None:
            return None
        ip = self.root
        while True:
            if ip.key < key and ip.right_child is not None:
                ip = ip.right_child
            elif key < ip.key and ip.left_child is not None:
                ip = ip.left_child
            else:
                break
        return ip
