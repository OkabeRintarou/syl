#include <cstdio>

double arr[1000];

int main()
{
	arr[0] = 1.0;
	for(int i = 1;i < 1000;i++){
		if(i & 1){
			arr[i] = arr[i - 1] + -1.0 / (i + 1);
		}else{
			arr[i] = arr[i - 1] + 1.0 / (i + 1);
		}
	} 

	int m,n;
	scanf("%d",&m);
	for(int i = 0;i < m;i++){
		scanf("%d",&n);
		printf("%.2lf\n",arr[n-1]);
	}
	return 0;
}
