#include <iostream>
using namespace std;

void merge(int A[],int p,int q,int r)
{
	int * n1 = new int[q - p + 1];
	int * n2 = new int[r - q];
	for(int i = p; i <= q;i++){
		n1[i-p] = A[i]; 
	}
	for(int i = q + 1; i <= r;i++){
		n2[i - q - 1] = A[i];
	}

	int i = 0;
	int j = 0;
	for(int k = p;k <= r;k++){
		if(i == q - p + 1){
			memmove(&A[k],n2 + j,r - q -j);
			break;
		}
		else if(j == r - q){
			memmove(&A[k],n1 + i,q - p - i + 1);
			break;
		}
		if (n1[i] < n2[j]){
			A[k] = n1[i];
			i++;
		}
		else{
			A[k] = n2[j];
			j++;
		}
	}
}

void merge_sort(int A[],int p,int r)
{
	if(p < r){
		int q = (p + r) / 2;
		merge_sort(A,p,q);
		merge_sort(A,q+1,r);
		merge(A,p,q,r);
	}
}

int main()
{
	int A[] = {5,2,4,7,1,3,2,6};
	merge_sort(A,0,7);
	for(int i = 0; i < 8;i++){
		cout<<A[i]<<' ';
	}
	return 0;
}