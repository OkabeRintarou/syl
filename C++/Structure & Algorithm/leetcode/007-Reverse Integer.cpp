class Solution {
public:
    int reverse(int x) {
        static const int overflow = INT_MAX / 10;
	    int ret = 0;
	    int n = x;
	    if (x  < 0){
		    n = -x;
	    }

	    while (n > 0){
		    if (ret > overflow)return 0;
		    ret = ( n % 10) + (ret * 10);
		    n /= 10;
	    }
	    return  x < 0 ? -ret : ret;
    }
};