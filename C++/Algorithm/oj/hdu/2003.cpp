#include <cstdio>

int main()
{
	double v;
	while(scanf("%lf",&v) != EOF){
		printf("%.2lf\n",v < 0 ? -v : v);
	}
	return 0;
}
