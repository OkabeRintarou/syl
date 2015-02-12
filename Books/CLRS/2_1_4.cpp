#include <iostream>
using namespace std;

void add_A_B(int A[],int B[],int C[],int N)
{
	for(int i = N - 1 ; i >= 0; i--){
		C[i+1] += (A[i] + B[i]);
		if(C[i+1] > 1){
			C[i+1] -= 2;
			C[i] += 1;
		}		
	}

	for(int i = 0; i < N+1;i++)
	{
		cout<<C[i]<<' ';
	}
}

int main()
{
	int A[] = {1,1,0,0,1,1,1,1};
	int B[] = {0,1,0,1,1,1,0,0};
	int C[] = {0,0,0,0,0,0,0,0,0};
	add_A_B(A,B,C,8);

	return 0;
}