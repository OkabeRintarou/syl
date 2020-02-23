#include <random>
#include <memory>
#include <gtest/gtest.h>
#include "btree.h"

TEST(BtreeTest, SeqInsert) {
  std::unique_ptr<BTree> btree(MakeTree(2));
  for (int i = 0; i < 10000; i++) {
    btree->insert(i, i * 100);
  }
  int value;
  for (int i = 0; i < 10000; i++) {
    value = -1;
    ASSERT_TRUE(btree->find(i, value));
    ASSERT_EQ(value, i * 100);
  }
}

TEST(BtreeTest, SeqTest2) {
  std::unique_ptr<BTree> btree(MakeTree(2));
  int value;
  for (int i = 20000; i > 0; i--) {
    btree->insert(i, i * 100);
  }
  ASSERT_EQ(btree->size(), 20000);
  for (int i = 1; i <= 20000; i++) {
    value = -1;
    ASSERT_TRUE(btree->find(i, value));
    ASSERT_EQ(value, i * 100);
  }
}

TEST(BtreeTest, SeqTest3) {
  std::unique_ptr<BTree> btree(MakeTree(2));
  for (int i = 0; i < 10000; i++) {
    btree->insert(i, i * 100);
  }
  for (int i = 20000; i > 10000; i--) {
    btree->insert(i, i * 100);
  }
  ASSERT_EQ(btree->size(), 20000);
  int value;
  for (int i = 0; i < 20000; i++) {
    if (i != 10000) {
      ASSERT_TRUE(btree->find(i, value));
      ASSERT_EQ(value, i * 100);
    }
  }
}

TEST(BtreeTest, SeqTest4) {
  std::unique_ptr<BTree> btree(MakeTree(2));
  for (int i = 0; i < 10000; i++) {
    btree->insert(i, i * 100);
  }
  for (int i = 0; i < 10000; i++) {
    btree->insert(i, i * 100);
  }
  ASSERT_EQ(btree->size(), 10000);
  int value;
  for (int i = 0; i < 10000; i++) {
    ASSERT_TRUE(btree->find(i, value));
    ASSERT_EQ(value, i * 100);
  }
}

TEST(BTreeTest, Remove1) {
  std::unique_ptr<BTree> btree(MakeTree(2));
  for (int i = 0; i < 10000; i++) {
    btree->insert(i, i * 100);
  }
  for (int i = 0; i < 10000; i++) {
    ASSERT_TRUE(btree->remove(i));
  }
  ASSERT_TRUE(btree->is_empty());
}

TEST(BTreeTest, Random1) {
  std::random_device rd;
  std::mt19937 g(rd());
  std::unique_ptr<BTree> btree(MakeTree(50));
  std::vector<int> keys(100000 + (g() % 100000));
  std::iota(keys.begin(), keys.end(), 0);
  std::shuffle(keys.begin(), keys.end(), g);
  for (int i = 0; i < keys.size(); i++) {
    btree->insert(keys[i], keys[i] * 100);
  }
  auto half = keys.size() / 2;
  int value;
  for (int i = 0; i < half; i++) {
    ASSERT_TRUE(btree->remove(keys[i]));
    ASSERT_FALSE(btree->find(keys[i], value));
  }
  ASSERT_EQ(keys.size() - half, btree->size());
  for (int i = half; i < keys.size(); i++) {
    ASSERT_TRUE(btree->find(keys[i], value));
    ASSERT_EQ(value, keys[i] * 100);
    ASSERT_TRUE(btree->remove(keys[i]));
  }
  ASSERT_TRUE(btree->is_empty());
}

TEST(BTreeTest, Random2) {
  std::random_device rd;
  std::mt19937 g(rd());
  std::unique_ptr<BTree> btree(MakeTree(50));
  std::vector<int> keys(100000 + (g() % 100000));
  std::iota(keys.begin(), keys.end(), 0);
  std::shuffle(keys.begin(), keys.end(), g);
  for (int i = 0; i < keys.size(); i++) {
    btree->insert(keys[i], keys[i] * 100);
  }
  std::vector<int> keys2(keys);
  std::shuffle(keys2.begin(), keys2.end(), g);
  auto half = keys2.size() / 2;
  int value;
  for (int i = 0; i < half; i++) {
    ASSERT_TRUE(btree->remove(keys2[i]));
    ASSERT_FALSE(btree->find(keys2[i], value));
  }
  ASSERT_EQ(keys2.size() - half, btree->size());
  for (int i = half; i < keys2.size(); i++) {
    ASSERT_TRUE(btree->find(keys2[i], value));
    ASSERT_EQ(value, keys2[i] * 100);
    ASSERT_TRUE(btree->remove(keys2[i]));
  }
  ASSERT_TRUE(btree->is_empty());
}

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}