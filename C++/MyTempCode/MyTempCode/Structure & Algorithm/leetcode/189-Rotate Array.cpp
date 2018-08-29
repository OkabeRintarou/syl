// 30 ms
void rotate(int nums[], int n, int k)
{
	k %= n;
	if (k == 0)return;
	int * p = new int[k];
	std::memmove(p, nums + n - k, sizeof(int) * k );
	std::memmove(nums + k, nums, sizeof(int)*(n - k));
	std::memmove(nums, p, k * sizeof(int));
	delete[] p;
}