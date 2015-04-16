string longestCommonPrefix(vector<string>& strs) 
{
	string ans;
	if (strs.empty()){
		return ans;
	}
	
	int minSize = strs[0].length();
	for (int i = 1; i < strs.size(); ++i){
		if (strs[i].length() < minSize){
			minSize = strs[i].length();
		}
	}

	int j = 0; 
	while (j < minSize){
		bool same = true;
		char c = strs[0][j];
		for (int i = 1; i < strs.size(); ++i){
			if (strs[i][j] != c){
				same = false;
				break;
			}
		}
		if (same){
			ans.push_back(c);
		}
		else{
			break;
		}
		
		++j;
	}

	return ans;
}