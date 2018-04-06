#include <cstdio>
#include <cmath>

int main()
{
	int n,m;
	while(scanf("%d%d",&n,&m) != EOF){
		double sum = n;
		double v = n;
		for(int i = 1;i < m;i++){
			v = sqrt(v);
			sum += v;
		}	
		printf("%.2lf\n",sum);
	}
	return 0;
}
