#include <ctime>
#include <cstdlib>
#include <numeric>
#include <iostream>
#include <vector>
#include <list>
#include <unordered_map>
#include <map>
#include <algorithm>
#include <iterator>

using namespace std;

/*----------插入排序------------*/
// 平均时间复杂度:O(n ^ 2)
// 最差时间复杂度:O(n ^ 2)
// 空间复杂度   ：O(n ^ 2)
template<typename T>
void Insert_Sort(std::vector<T> & v)
{
	int size = v.size();
	if (size < 2){
		return;
	}

	for (int i = 1; i < size; ++i){
		int cur = v[i];
		int j = i - 1;
		while (j >= 0 && v[j] > cur){
			v[j + 1] = v[j];
			j--;
		}
		// j == -1 || v[j] <= cur
		v[j + 1] = cur;
		
	}
}

/*--------------归并排序----------------*/
// 平均时间复杂度：O(n * log(n))
// 最差时间复杂度：O(n * log(n))
// 空间复杂度	   ：数组 = O(n) + 0(log(n)),链表 = O(1)
template<typename T>
static void Merge(std::vector<T> & v, int p, int q, int r)
{
	int n1 = q - p + 1;
	int n2 = r - q;
	std::vector<T> left(v.begin() + p, v.begin() +(q + 1));
	std::vector<T> right(v.begin() + (q + 1),v.begin() + (r + 1));

	int i = 0;
	int j = 0;
	for (int k = p; k <= r; ++k){
		if (i == left.size()){
			std::copy(right.begin() + j, right.end(), v.begin() + k);
			break;
		}
		else if (j == right.size()){
			std::copy(left.begin() + i, left.end(), v.begin() + k);
			break;
		}

		if (left[i] < right[j]){
			v[k] = left[i];
			++i;
		}
		else{
			v[k] = right[j];
			++j;
		}
		
	}
}

template<typename T>
static void Merge_Sort_Impl(std::vector<T> & v, int p, int r)
{
	if (p < r){
		int q = (p + r) / 2;
		Merge_Sort_Impl(v, p, q);
		Merge_Sort_Impl(v, q + 1, r);
		Merge(v, p, q, r);
	}
}

template<typename T>
void Merge_Sort(std::vector<T> & v)
{
	Merge_Sort_Impl(v, 0, v.size() - 1);
}
	

/*------------------快速排序----------------------*/
// 平均时间复杂度：O(n * log(n))
// 最差时间复杂度：O(n ^ 2)
// 空间复杂度	   ：
template<typename T>
static int Quick_Sort_Partition(std::vector<T> & v, int p, int r)
{
	int x = v[r];
	int i = p - 1;
	for (int j = p; j < r; ++j){
		if (v[j] < x){
			++i;
			std::swap(v[i], v[j]);
		}
	}

	std::swap(v[i + 1], v[r]);

	return i + 1;
}

template<typename T>
static void Quick_Sort_Impl(std::vector<T> & v, int p, int r)
{
	if (p < r){
		int q = Quick_Sort_Partition(v, p, r);
		Quick_Sort_Impl(v, p, q - 1);
		Quick_Sort_Impl(v, q + 1, r);
	}
}

template<typename T>
void Quick_Sort(std::vector<T> & v)
{
	Quick_Sort_Impl(v, 0, v.size() - 1);
}

/*-------------------堆排序------------------*/
static inline int Heap_Parent(int i)
{
	return  (i - 1) / 2;
}

static inline int Heap_Left(int i)
{
	return 2 * i + 1;
}

static inline int Heap_Right(int i)
{
	return 2 * i + 2;
}

template<typename T>
static void Heap_Max_Heapify(std::vector<T> & v, int len,int i)
{
	int left = Heap_Left(i);
	int right = Heap_Right(i);
	int largest = -1;
	if (left <= len && v[left] > v[i]){
		largest = left;
	}
	else{
		largest = i;
	}
	if (right <= len && v[right] > v[largest]){
		largest = right;
	}
	
	if (largest != i){
		std::swap(v[i], v[largest]);
		Heap_Max_Heapify(v, len,largest);
	}
}

template<typename T>
static void Build_Max_Heap(std::vector<T> & v)
{
	int len = v.size() - 1;
	int start = Heap_Parent(len);
	for (int i = start; i >= 0; --i){
		Heap_Max_Heapify(v,len, i);
	}
}

template<typename T>
void Heap_Sort(std::vector<T> & v)
{
	Build_Max_Heap(v);
	int len = v.size() - 1;
	for (int i = len; i > 0; --i){
		std::swap(v[0], v[i]);
		--len;
		Heap_Max_Heapify(v, len, 0);
	}
}
	
template<typename T>
void PrintVec(const std::vector<T> & v)
{
	std::copy(v.begin(), v.end(), std::ostream_iterator<T>(std::cout, " "));
	cout << '\n';
}

// return milliseconds elapsed
double CalcTimeElapsed(clock_t start)
{
	return static_cast<double>(clock() - start) /
		static_cast<double>(CLOCKS_PER_SEC) * 1000.0;
}

////////////////////////////////////////////////////////////////
//// MAXN = 100000
//// 插入排序:409517ms,归并排序:3996ms
////
//// MAXN = 1000000
////                  归并排序：40427ms
////////////////////////////////////////////////////////////////

static const int MAXN = 50;
int main(int argc, char * argv[])
{
	
	std::vector<int> v(MAXN);
	std::iota(v.begin(), v.end(), 0);
	std::random_shuffle(v.begin(), v.end());

	clock_t start = clock();
	/*Insert_Sort(v);
	std::cout << "Insert Sort: " << CalcTimeElapsed(start) << "ms" << std::endl;*/

	std::random_shuffle(v.begin(), v.end());
	
	PrintVec(v);
	start = clock();
	Heap_Sort(v);
	std::cout << "Merge Sort: " << CalcTimeElapsed(start) << "ms" << std::endl;

	PrintVec(v);
	return 0;
}

