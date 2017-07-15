#include <cstdio>

int main()
{
	int score;
	while(scanf("%d",&score) != EOF){
		if(score < 0 || score > 100){
			printf("Score is error!\n");
		}else if(score <= 59){
			printf("E\n");
		}else if(score <= 69){
			printf("D\n");
		}else if(score <= 79){
			printf("C\n");
		}else if(score <= 89){
			printf("B\n");
		}else{
			printf("A\n");
		}
	}
	return 0;
}
