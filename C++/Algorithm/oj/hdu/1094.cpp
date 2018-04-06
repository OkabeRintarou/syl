#include <cstdio>

int main()
{
	int n;
	long long sum;
	long long v;
	while(scanf("%d",&n) != EOF){
		sum = 0;
		for(int i = 0;i < n;i++){
			scanf("%lld",&v);
			sum += v;
		}
		printf("%lld\n",sum);
		
	}
}
