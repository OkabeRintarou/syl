#if 0 
时间：24ms
#endif

vector<int> twoSum(vector<int> &numbers, int target) {
    using IntIntMap = std::unordered_map<int, int>;

	vector<int> ret(2);
	IntIntMap table;

	for (int i = 0; i < numbers.size(); ++i){
		int brother = target - numbers[i];
		if (table.find(brother) != table.end()){
			ret[0] = table[brother];
			ret[1] = i + 1;
			return ret;
		}
		else{
			table[numbers[i]] = i + 1;
		}
	}

	return ret;
}