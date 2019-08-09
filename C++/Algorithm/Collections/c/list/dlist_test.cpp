#include <algorithm>
#include <dlist.h>
#include <gtest/gtest.h>
#include <numeric>
#include <random>
#include <set>
#include <vector>

static int is59(void *v) {
  int num = *(int *)v;
  return num == 59;
}

TEST(DoubleLinkedList, Simple) {
  struct dlist_t *list = new_dlist();
  ASSERT_NE(list, nullptr);
  EXPECT_TRUE(dlist_is_empty(list));
  EXPECT_EQ(dlist_size(list), 0);

  for (int i = 0; i < 100; i++) {
    int *value = (int *)malloc(sizeof(int));
    *value = i;
    dlist_insert_head(list, value);
  }

  EXPECT_EQ(dlist_size(list), 100);

  for (int i = 100; i < 200; i++) {
    int *value = (int *)malloc(sizeof(int));
    *value = i;
    dlist_insert_tail(list, value);
  }
  EXPECT_EQ(dlist_size(list), 200);

  int *data;
  struct item_t *head = dlist_get_head(list);
  ASSERT_NE(head, nullptr);
  data = (int *)head->data;
  EXPECT_EQ(*data, 99);
  struct item_t *tail = dlist_get_tail(list);
  ASSERT_NE(tail, nullptr);
  data = (int *)tail->data;
  EXPECT_EQ(*data, 199);

  struct item_t *item59 = dlist_find(list, is59);
  ASSERT_NE(item59, nullptr);
  ASSERT_NE(item59->data, nullptr);
  ASSERT_EQ(*(int *)item59->data, 59);

  dlist_remove(item59);
  free_item(item59);

  ASSERT_EQ(dlist_size(list), 199);
  dlist_free(list);
}

class DoubleLinkedListTest : public ::testing::Test {
protected:
  void SetUp() override {
    list = new_dlist();
    std::random_device rd;

    std::set<int> removed;
    std::vector<int> numbers(5000);
    std::iota(numbers.begin(), numbers.end(), 0);
    std::vector<struct item_t *> items;
    // generate remove numbers
    {
      std::vector<int> nums;
      nums.resize(10000);
      std::iota(nums.begin(), nums.end(), 0);
      std::shuffle(nums.begin(), nums.end(), std::mt19937(rd()));
      removed.insert(nums.begin(), nums.begin() + 5000);
    }

    for (const int i : numbers) {
      int *v = (int *)malloc(sizeof(int));
      *v = i;
      items.push_back(dlist_insert_tail(list, v));
      if (removed.find(i) != removed.end()) {
        removed_items.push_back(items.back());
      }
    }

    std::vector<int> numbers2(5000);
    std::iota(numbers2.begin(), numbers2.end(), 5000);

    for (const int i : numbers2) {
      int *v = (int *)malloc(sizeof(int));
      *v = i;
      auto index = rd() % items.size();
      auto *item = items[index];
      struct item_t *ni;
      if (rd() % 2) {
        ni = dlist_insert_after(list, item, v);
      } else {
        ni = dlist_insert_before(list, item, v);
      }
      if (removed.find(i) != removed.end()) {
        removed_items.push_back(ni);
      }
    }
  }

  void TearDown() override { dlist_free(list); }

  struct dlist_t *list;

  std::vector<struct item_t *> removed_items;
};

TEST_F(DoubleLinkedListTest, Random) {
  EXPECT_EQ(dlist_size(list), 10000);
  for (const auto item : removed_items) {
    dlist_remove(item);
    free_item(item);
  }
  EXPECT_EQ(dlist_size(list), 5000);
}

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
