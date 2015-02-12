#include <iostream>
using std::cout;

void insert_sort(int A[],int N)
{
	for(int j = 0; j < N;j++){

		int key = A[j];
		int i = j - 1;

		while(i >= 0 && A[i] < key){
			A[i + 1] = A[i];
			i--;
		}
		A[i+1] = key;
	}
}

int main(int argc,char * argv[])
{
	int A[] = {31,41,59,26,41,58};
	const int N = sizeof(A) / sizeof(A[0]);
	insert_sort(A,N);
	for(int i = 0; i < N;i++){
		cout<<A[i]<<' ';
	}
}