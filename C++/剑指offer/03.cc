/**
*  struct ListNode {
*        int val;
*        struct ListNode *next;
*        ListNode(int x) :
*              val(x), next(NULL) {
*        }
*  };
*/
class Solution {
public:
    vector<int> printListFromTailToHead(ListNode *head) {
        vector<int> v;
        if (!head) {
            return v;
        }
        helper(v, head);
        return v;
    }

private:
    void helper(vector<int> &v, ListNode *node) {
        if (!node->next) {
            v.push_back(node->val);
        } else {
            helper(v, node->next);
            v.push_back(node->val);
        }
    }
};