class Solution {
public:
    bool Find(int target, vector <vector<int>> array) {
        if (array.empty() || array[0].empty()) {
            return false;
        }

        int height = (int) array.size(), width = (int) array[0].size();
        int x = 0, y = width - 1;

        while (x < height && y >= 0) {
            if (array[x][y] == target) {
                return true;
            } else if (array[x][y] > target) {
                y--;
            } else {
                x++;
            }
        }

        return false;
    }
};