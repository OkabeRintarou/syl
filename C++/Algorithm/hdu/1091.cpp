#include <cstdio>

int main()
{
	long long a,b;
	while(scanf("%lld%lld",&a,&b) != EOF){
		if(a == 0 && b == 0)break;
		printf("%lld\n",a+b);
	}
	return 0;
}
