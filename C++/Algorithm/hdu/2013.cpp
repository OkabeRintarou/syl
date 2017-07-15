#include <cstdio>

int main()
{
	int n;
	while(scanf("%d",&n) != EOF){
		int t = 1;
		while(--n){
			t = 2 * t + 2;
		}
		printf("%d\n",t);
	}
	return 0;
}
