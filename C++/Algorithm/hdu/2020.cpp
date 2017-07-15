#include <cstdio>
#include <cstdlib>

int arr[105];

int abs_comp(const void* a,const void* b)
{
	int lhs = *(int*)a;
	int rhs = *(int*)b;
	if(lhs < 0)lhs = -lhs;
	if(rhs < 0)rhs = -rhs;
	return rhs - lhs;
}

int main()
{
	int n;
	while(scanf("%d",&n) != EOF){
		if(n == 0)break;
		for(int i = 0;i < n;i++){
			scanf("%d",&arr[i]);
		}
		qsort(arr,n,sizeof(int),abs_comp);
		for(int i = 0;i < n;i++){
			if(i)printf(" %d",arr[i]);
			else printf("%d",arr[i]);
		}
		printf("\n");
	}
	return 0;
}
