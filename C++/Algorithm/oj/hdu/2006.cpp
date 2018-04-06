#include <cstdio>

int main()
{
	int n,mul,a;
	while(scanf("%d",&n) != EOF){
		mul = 1;
		while(n--){
			scanf("%d",&a);
			if(a & 1)mul *= a;
		}
		printf("%d\n",mul);
	}
	return 0;
}
