#include <cstdio>

int numbers[] = {153,370,371,407};

int main()
{
	int m,n;
	int count = 0;
	while(scanf("%d%d",&m,&n) != EOF){
		if(m > n){
			int t = m;m = n;n = t;
		}
		count = 0;
		for(int i = 0;i < 4;i++){
			if(numbers[i] >= m && numbers[i] <= n){
				if(count == 0)printf("%d",numbers[i]);
				else printf(" %d",numbers[i]);
				++count;
			}
		}
		if(count)printf("\n");
		else printf("no\n");
	}
	return 0;
}
