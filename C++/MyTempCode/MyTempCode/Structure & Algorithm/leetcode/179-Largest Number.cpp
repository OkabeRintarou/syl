class Solution {
public:
	string largestNumber(vector<int> &num) {
		string ret; 
		
		sort(num.begin(), num.end(), [](int a, int b){
			string lhs = to_string(a) + to_string(b);
			string rhs = to_string(b) + to_string(a);
			for (int i = 0; i < lhs.size(); i++){
				if (lhs[i] > rhs[i]){
					return true;
				}
				else if (lhs[i] < rhs[i]){
					return false;
				}
			}
			return false;
		});
		
		for (auto && i : num){
			ret += to_string(i);
		}
		if (ret[0] == '0'){
			return "0";
		}
		return ret;
	}
};