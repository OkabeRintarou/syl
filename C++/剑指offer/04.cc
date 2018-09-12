class Solution {
public:
    TreeNode *reConstructBinaryTree(vector<int> pre, vector<int> vin) {
        if (pre.empty() || vin.empty() || pre.size() != vin.size()) {
            return NULL;
        }
        auto *ans = new TreeNode(pre[0]);
        auto it = find(vin.begin(), vin.end(), pre[0]);
        int len = (int) (it - vin.begin());
        ans->left = helper(pre, 1, len,
                           vin, 0, len - 1);
        ans->right = helper(pre, len + 1, (int) pre.size() - 1,
                            vin, len + 1, (int) vin.size() - 1);
        return ans;
    }

private:
    TreeNode *helper(const vector<int> &pre, int h1, int e1,
                     const vector<int> &vin, int h2, int e2) {
        if (e1 < h1 || e2 < h2 ||
            e1 < 0 || e2 < 0 ||
            h1 >= (int) pre.size() || h2 >= (int) vin.size()) {
            return NULL;
        }

        auto *ans = new TreeNode(pre[h1]);
        auto it = find(vin.begin() + h2, vin.begin() + e2 + 1, pre[h1]);
        int len = (int) (it - (vin.begin() + h2));
        ans->left = helper(pre, h1 + 1, h1 + len,
                           vin, h2, h2 + len - 1);
        ans->right = helper(pre, h1 + len + 1, e1,
                            vin, h2 + len + 1, e2);
        return ans;
    }
};
