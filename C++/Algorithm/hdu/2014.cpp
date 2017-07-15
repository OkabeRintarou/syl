#include <cstdio>
#include <climits>

int main()
{
	int n;
	while(scanf("%d",&n) != EOF){
		int sum = 0,max = INT_MIN,min = INT_MAX;
		int v;
		for(int i = 0;i < n;i++){
			scanf("%d",&v);
			sum += v;
			if(v > max)max = v;
			if(v < min)min = v;
		}
		printf("%.2lf\n",(sum - max - min) * 1.0 / (n - 2));
	}
	return 0;
}
