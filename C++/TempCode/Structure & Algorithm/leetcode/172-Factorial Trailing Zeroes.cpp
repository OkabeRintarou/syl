class Solution {
public:
    int trailingZeroes(int n) {
        int ans = 0;
	    do{
	        ans += n / 5;
	    }while(n /= 5);
	    
	    return ans;
    }
};