class Solution {
public:
	int singleNumber(int A[], int n) {
		while (--n > 0){
			A[0] ^= A[n];
		}
		return A[0];
	}
};