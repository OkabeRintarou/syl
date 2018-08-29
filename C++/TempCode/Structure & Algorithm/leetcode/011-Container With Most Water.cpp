int maxArea(vector<int>& height) 
{
	int j = height.size() - 1, i = 0, mx = 0;

	while (i < j){
		mx = max(mx, (j - i) * min(height[j] , height[i]));

		if (height[i] < height[j]){
			++i;
		}
		else{
			--j;
		}
	}

	return mx;
}