int lengthOfLongestSubstring(string s)
{
	unordered_map<char, int> mp;

	int longest = 0, m = 0;

	for (int i = 0; i < s.length(); ++i){
		if (mp.find(s[i]) != mp.end()){
			m = max(mp[s[i]] + 1, m);
		}
		
		mp[s[i]] = i;
		
		longest = max(longest, i - m + 1);
	}

	return longest;
}