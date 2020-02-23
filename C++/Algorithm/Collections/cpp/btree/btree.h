#pragma once
#include <cstddef>

class BTree {
public:
  virtual bool insert(int key, int value) = 0;
  virtual bool find(int key, int& value) = 0;
  virtual bool remove(int key) = 0;
  std::size_t size() const { return _size; }
  bool is_empty() const { return _size == 0; }
protected:
  std::size_t _size {0};
};

BTree* MakeTree(int t);
