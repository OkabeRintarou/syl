#include <bits/stdc++.h>

using namespace std;

class Solution {
public:
  vector<int> twoSum(vector<int> &nums, int target) {

    std::unordered_map<int, int> mp;
    for (int i = 0, end = static_cast<int>(nums.size()); i < end; i++) {
      auto it = mp.find(target - nums[i]);
      if (it != std::end(mp)) {
        return vector<int>{it->second, i};
      } else {
        mp.emplace(nums[i], i);
      }
    }
    return vector<int>();
  }
};

int main() {

  std::vector<int> nums{2, 7, 11, 15};
  auto v = Solution().twoSum(nums, 9);
  std::cout << v[0] << " " << v[1] << '\n';
  return 0;
}
