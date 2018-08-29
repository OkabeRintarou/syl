pair<int,int> longestPalindrome(const string & s, int left, int right)
{
	while (left >= 0 && right < s.length() 
			&& s[left] == s[right]){
		--left;
		++right;
	}

	return pair<int, int>(left + 1, right);

}

string longestPalindrome(string s) 
{
	int len = s.length();
	int maxLen = 0;
	pair<int, int> rge;

	for (int i = 0; i < 2 * len - 1; ++i){
		int left  = i / 2;
		int right = i / 2;
		if (i & 1) ++right;

		pair<int,int> r = longestPalindrome(s, left, right);
		if (maxLen < r.second - r.first){
			rge = r;
			maxLen = r.second - r.first;
		}
	}

	return string(s.begin() + rge.first, s.begin() + rge.second);
}