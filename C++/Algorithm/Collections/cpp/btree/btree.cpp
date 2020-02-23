#include <cassert>
#include <vector>
#include <algorithm>
#include "btree.h"

struct Node {
  Node(int t);

  virtual bool insert(int key, int value) = 0;

  virtual Node *split(int *mid_key) = 0;

  virtual bool find(int key, int &value) = 0;

  virtual int remove(int key, bool &removed) = 0;

  virtual void merge(int key, Node *other) = 0;

  void right_rotate(int key_index, Node *lhs, Node *rhs);

  void left_rotate(int key_index, Node *lhs, Node *rhs);

  int successor_key(int key) const;

  bool is_empty() const { return keys.size() == 1; }

  bool is_full() const { return keys.size() == 2 * degree; }

  bool is_half_full() const { return keys.size() == degree; }

  bool is_more_than_half_full() const { return keys.size() >= degree + 1; }

  virtual bool is_leaf() const = 0;

  virtual ~Node() = default;

  std::vector<int> keys;
  int degree;
};

Node::Node(int t) {
  degree = t;
  keys.reserve(2 * t);
  keys.resize(1);
}

int Node::successor_key(int key) const {
  assert(keys.size() > 2 && key <= keys[1]);
  return key < keys[1] ? keys[1] : keys[2];
}

struct LeafNode : public Node {
  LeafNode(int t);

  bool is_leaf() const override;

  bool insert(int key, int value) override;

  bool find(int key, int &value) override;

  int remove(int key, bool &removed) override;

  void merge(int key, Node *other) override;

  Node *split(int *mid_key) override;

  ~LeafNode() override;

  std::vector<int> values;
};

LeafNode::LeafNode(int t) : Node(t) {
  values.reserve(2 * t);
  values.resize(1);
}

LeafNode::~LeafNode() {

}

bool LeafNode::is_leaf() const {
  return true;
}


/**
 * insert (key, value) into current leaf node
 * @return true iff key doesn't exist in current node
 * @precondition current node is not full
 */
bool LeafNode::insert(int key, int value) {
  assert(!is_full());
  auto it = std::lower_bound(keys.begin() + 1, keys.end(), key);
  if (it != keys.end() && *it == key) {
    int index = it - keys.begin();
    values[index] = value;
    return false;
  } else {
    keys.insert(it, key);
    values.insert(values.begin() + (it - keys.begin()), value);
    return true;
  }
}

/**
 * split current leaf node
 * @param mid_key store the middle key
 * @return a new allocated leaf node
 * @precondition current node is full
 */
Node *LeafNode::split(int *mid_key) {
  assert(is_full());
  const int index = degree;
  auto new_node = new LeafNode(degree);
  *mid_key = keys[index];
  std::copy(keys.begin() + index, keys.end(), std::back_inserter(new_node->keys));
  std::copy(values.begin() + index, values.end(), std::back_inserter(new_node->values));
  keys.resize(degree);
  values.resize(degree);
  return new_node;
}

bool LeafNode::find(int key, int &value) {
  auto it = std::lower_bound(keys.begin() + 1, keys.end(), key);
  if (it != keys.end() && *it == key) {
    int index = it - keys.begin();
    value = values[index];
    return true;
  }
  return false;
}

void LeafNode::merge(int, Node *other) {
  assert(is_half_full() && other->is_half_full());
  auto other_leaf = dynamic_cast<LeafNode *>(other);
  assert(other_leaf != nullptr);
  keys.insert(keys.end(), other_leaf->keys.begin() + 1, other_leaf->keys.end());
  values.insert(values.end(), other_leaf->values.begin() + 1, other_leaf->values.end());
}

int LeafNode::remove(int key, bool &removed) {

  auto it = std::lower_bound(keys.begin() + 1, keys.end(), key);
  if (it != keys.end() && *it == key) {
    auto value_iter = values.begin() + (it - keys.begin());
    if (it + 1 != keys.end()) {
      key = *(it + 1);
    }
    keys.erase(it);
    values.erase(value_iter);
    removed = true;
    return key;
  }
  removed = false;
  return -1;
}

struct InnerNode : public Node {
  InnerNode(int t);
  ~InnerNode() override;

  std::vector<Node *> children;

  bool is_leaf() const override;

  bool insert(int key, int value) override;

  bool find(int key, int &value) override;

  bool find(int key, Node *&lhs, Node *&rhs, int &key_index);

  int remove(int key, bool &removed) override;

  Node *split(int *mid_key) override;

  void merge(int key, Node *other) override;
};

bool InnerNode::is_leaf() const {
  return false;
}

InnerNode::InnerNode(int t) : Node(t) {
  children.reserve(2 * t);
}

InnerNode::~InnerNode() {

}

bool InnerNode::insert(int key, int value) {
  auto it = std::upper_bound(keys.begin() + 1, keys.end(), key);
  auto child_iter = children.begin() + (it - keys.begin());
  --child_iter;
  Node *child = *child_iter;
  if (child->is_full()) {
    int mid_key;
    Node *new_child = child->split(&mid_key);
    keys.insert(it, mid_key);
    auto next_child_iter = children.insert(child_iter + 1, new_child);
    if (mid_key < key) {
      child = *next_child_iter;
    }
  }
  return child->insert(key, value);
}

bool InnerNode::find(int key, int &value) {
  auto it = std::upper_bound(keys.begin() + 1, keys.end(), key);
  auto child_iter = children.begin() + (it - keys.begin());
  --child_iter;
  Node *child = *child_iter;
  return child->find(key, value);
}

/**
 * split current inner node
 * @param mid_key store the middle key
 * @return a new allocated inner node
 * @precondition current node is full
 */
Node *InnerNode::split(int *mid_key) {
  assert(is_full());
  const int index = degree;
  auto new_node = new InnerNode(degree);
  *mid_key = keys[index];
  std::copy(keys.begin() + index + 1, keys.end(), std::back_inserter(new_node->keys));
  std::copy(children.begin() + index, children.end(), std::back_inserter(new_node->children));
  keys.resize(degree);
  children.resize(degree);
  return new_node;
}

bool InnerNode::find(int key, Node *&lhs, Node *&rhs, int &key_index) {
  bool ret = false;
  auto it = std::lower_bound(keys.begin() + 1, keys.end(), key);
  auto rhs_child_iter = children.begin() + (it - keys.begin());
  if (it == keys.end()) {
    --rhs_child_iter;
    --it;
  } else if (*it == key) {
    ret = true;
  }
  auto lhs_child_iter = rhs_child_iter - 1;
  lhs = *lhs_child_iter;
  rhs = *rhs_child_iter;
  key_index = it - keys.begin();
  return ret;
}

void InnerNode::merge(int key, Node *other) {
  assert(is_half_full() && other->is_half_full());
  auto inner = dynamic_cast<InnerNode *>(other);
  assert(inner != nullptr);
  keys.push_back(key);
  auto key_iter = inner->keys.begin() + 1;
  auto child_iter = inner->children.begin();
  while (key_iter != inner->keys.end() && *key_iter == key) {
    ++key_iter;
    ++child_iter;
  }
  keys.insert(keys.end(), key_iter, inner->keys.end());
  children.insert(children.end(), child_iter, inner->children.end());
}

int InnerNode::remove(int key, bool &removed) {
  Node *lhs = nullptr;
  Node *rhs = nullptr;
  int key_index = -1;
  bool contain = find(key, lhs, rhs, key_index);
  assert(lhs != nullptr && rhs != nullptr && key_index >= 1);

  if (key < keys[key_index]) {
    // remove key from lhs
    if (lhs->is_more_than_half_full()) {
      return lhs->remove(key, removed);
    } else if (rhs->is_more_than_half_full()) {
      left_rotate(key_index, lhs, rhs);
      return lhs->remove(key, removed);
    } else {
      lhs->merge(keys[key_index], rhs);
      delete rhs;
      keys.erase(keys.begin() + key_index);
      children.erase(children.begin() + key_index);
      return lhs->remove(key, removed);
    }
  } else {
    // remove key from rhs
    if (rhs->is_more_than_half_full()) {
      int new_key = rhs->remove(key, removed);
      if (contain && removed) {
        keys[key_index] = new_key;
      }
      return new_key;
    } else if (lhs->is_more_than_half_full()) {
      right_rotate(key_index, lhs, rhs);
      return rhs->remove(key, removed);
    } else {
      lhs->merge(keys[key_index], rhs);
      delete rhs;
      keys.erase(keys.begin() + key_index);
      children.erase(children.begin() + key_index);
      return lhs->remove(key, removed);
    }
  }
}

void Node::right_rotate(int key_index, Node *lhs, Node *rhs) {
  assert(lhs->is_more_than_half_full());

  if (lhs->is_leaf()) {
    int key = lhs->keys.back();
    keys[key_index] = key;
    lhs->keys.pop_back();
    rhs->keys.insert(rhs->keys.begin() + 1, key);
    auto lhs_leaf = dynamic_cast<LeafNode *>(lhs);
    auto rhs_leaf = dynamic_cast<LeafNode *>(rhs);
    rhs_leaf->values.insert(rhs_leaf->values.begin() + 1, lhs_leaf->values.back());
    lhs_leaf->values.pop_back();
  } else {
    int key = keys[key_index];
    keys[key_index] = lhs->keys.back();
    lhs->keys.pop_back();
    rhs->keys.insert(rhs->keys.begin() + 1, key);
    auto lhs_inner = dynamic_cast<InnerNode *>(lhs);
    auto rhs_inner = dynamic_cast<InnerNode *>(rhs);
    rhs_inner->children.insert(rhs_inner->children.begin(), lhs_inner->children.back());
    lhs_inner->children.pop_back();
  }
}

void Node::left_rotate(int key_index, Node *lhs, Node *rhs) {
  assert(rhs->is_more_than_half_full());
  int key = keys[key_index];
  keys[key_index] = rhs->successor_key(key);
  rhs->keys.erase(rhs->keys.begin() + 1);
  lhs->keys.push_back(key);

  if (lhs->is_leaf()) {
    auto lhs_leaf = dynamic_cast<LeafNode *>(lhs);
    auto rhs_leaf = dynamic_cast<LeafNode *>(rhs);
    lhs_leaf->values.push_back(*(rhs_leaf->values.begin() + 1));
    rhs_leaf->values.erase(rhs_leaf->values.begin() + 1);
  } else {
    auto lhs_inner = dynamic_cast<InnerNode *>(lhs);
    auto rhs_inner = dynamic_cast<InnerNode *>(rhs);
    lhs_inner->children.push_back(rhs_inner->children.front());
    rhs_inner->children.erase(rhs_inner->children.begin());
  }
}

struct BTreeImpl : public BTree {
  BTreeImpl(int t);

  bool insert(int key, int value) override;

  bool find(int key, int &value) override;

  bool remove(int key) override;

  Node *root;
  int degree;
};

BTreeImpl::BTreeImpl(int t) {
  degree = t;
  root = new LeafNode(t);
}

bool BTreeImpl::insert(int key, int value) {
  if (root->is_full()) {
    int mid_key;
    Node *new_node = root->split(&mid_key);
    auto new_root = new InnerNode(degree);
    new_root->keys.push_back(mid_key);
    new_root->children.push_back(root);
    new_root->children.push_back(new_node);
    root = new_root;
  }
  bool ret = root->insert(key, value);
  if (ret) {
    ++_size;
  }
  return ret;
}

bool BTreeImpl::find(int key, int &value) {
  return root->find(key, value);
}

bool BTreeImpl::remove(int key) {
  bool removed;
  root->remove(key, removed);
  if (root->is_empty() && !root->is_leaf()) {
    auto inner = dynamic_cast<InnerNode*>(root);
    assert(inner != nullptr && inner->children.size() == 1);
    Node* new_root = inner->children[0];
    delete root;
    root = new_root;
  }
  if (removed) {
    --_size;
  }
  return removed;
}

BTree *MakeTree(int t) {
  return new BTreeImpl(t);
}