#include <cstdio>

int main()
{
	int n,pos,neg,zero;
	double v;
	while(scanf("%d",&n) != EOF){
		if(n == 0)break;
		pos = neg = zero = 0;
		for(int i = 0;i < n;i++){
			scanf("%lf",&v);
			if(v > 0)pos++;
			else if(v < 0)neg++;
			else zero++;
		}
		printf("%d %d %d\n",neg,zero,pos);
	}
	return 0;
}
