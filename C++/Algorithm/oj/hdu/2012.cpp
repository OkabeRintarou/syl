#include <cstdio>
#include <cmath>

int main()
{
	int x,y,n,count;
	while(scanf("%d%d",&x,&y) != EOF){
		if(x == 0 && y == 0)break;
		count = 0;
		for(int i = x;i <= y;i++){
			n = i * i + i + 41;
			int flag = 0;
			for(int j = 2;j < sqrt(n) + 1;j++){
				if(n % j == 0){
					flag = 1;
					break;
				}
			} 

			if(flag){
				count++;
				break;
			}
		}
		if(count){
			printf("Sorry\n");
		}else{
			printf("OK\n");
		}
	}
	return 0;
}
