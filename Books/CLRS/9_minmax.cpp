#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>

template<typename T>
std::pair<T, T> MinMax(const std::vector<T> & v)
{

	if (v.size() == 0)return std::pair<T, T>();
	else if (v.size() == 1){ return std::pair<T, T>(v[0], v[0]); }

	// v.size() >= 2
	std::pair<T, T> r;
	size_t i = 0;
	if (v.size() & 1){
		r.first = r.second = v[0];
		i = 1;
	}
	else{
		r.first = v[1];
		r.second = v[0];
		if (v[0] < v[1]){
			std::swap(r.first, r.second);
		}
		i = 2;
	}

	for (; i < v.size(); i += 2){
		int left = v[i + 1];
		int right = v[i];
		if (v[i] < v[i + 1]){
			std::swap(left, right);
		}
		if (left < r.first)r.first = left;
		if (right > r.second)r.second = right;
	}

	return r;
}

int main(int argc,char * argv[])
{
	std::pair<int,int> r = MinMax(std::vector<int>{-100,-1,0,111,1,2,3,4,5,6,7,8,9});
	std::cout << r.first <<' '<< r.second <<'\n';
	return 0;
}