#include <cstdio>

int main()
{
	int n;
	scanf("%d",&n);
	long long a,b;
	for(int i = 0;i < n;i++){
		scanf("%lld%lld",&a,&b);
		printf("%lld\n",a + b);
	}
	return 0;
}
