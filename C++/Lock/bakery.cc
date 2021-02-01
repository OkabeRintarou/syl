#include "lock.h"
#include <algorithm>
#include <cassert>
#include <iostream>
#include <thread>
#include <vector>

thread_local unsigned thread_id;

class BakeryLock : public Lock {
public:
  explicit BakeryLock(const unsigned concurrency);

  virtual void lock() override;
  virtual void unlock() override;

private:
  struct alignas(64) aligned_int {
    operator int() const { return value; }

    aligned_int() : value(0) {}
    aligned_int(int v) : value(v) {}

    int value;
  };
  std::vector<aligned_int> choosing_;
  std::vector<aligned_int> number_;
};

BakeryLock::BakeryLock(const unsigned concurrency) {
  assert(concurrency > 0);
  choosing_.resize(concurrency, 0);
  number_.resize(concurrency, 0);
}

void BakeryLock::lock() {

  const size_t i = thread_id;
  choosing_[i] = 1;
  number_[i] = 1 + *std::max_element(std::begin(number_), std::end(number_));
  choosing_[i] = 0;

  const size_t n = choosing_.size();
  for (size_t j = 0; j < n; j++) {
    while (choosing_[j] != 0) {
    }
    while (number_[j] != 0 &&
           ((number_[j] < number_[i]) || (number_[j] == number_[i] && j < i))) {
    }
  }
}

void BakeryLock::unlock() { number_[thread_id] = 0; }

uint64_t global_count = 0;

void thread_func(BakeryLock *lock, const unsigned id) {
  // each thread set its thread-local id
  thread_id = id;

  for (int i = 0; i < 100000; i++) {
    lock->lock();
    for (int j = 0; j < 10; j++) {
      global_count++;
    }
    lock->unlock();
  }
}

int main() {
  const unsigned int concurrency = std::thread::hardware_concurrency();
  std::cout << "concurrency = " << concurrency << std::endl;

  BakeryLock lock(concurrency);
  std::vector<std::thread> ts;
  ts.reserve(concurrency);

  for (unsigned i = 0; i < concurrency; i++) {
    ts.emplace_back(std::thread(thread_func, &lock, i));
  }

  for (std::thread &t : ts) {
    t.join();
  }

  std::cout << "global count: " << global_count << std::endl;
  return 0;
}
