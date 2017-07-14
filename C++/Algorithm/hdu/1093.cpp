#include <cstdio>

int main()
{
	int m,n;
	long long sum;
	long long v;
	scanf("%d",&m);
	while(m--){
		scanf("%d",&n);
		sum = 0;
		for(int i = 0;i < n;i++){
			scanf("%lld",&v);
			sum += v;
		}
		printf("%lld\n",sum);
	}
}
