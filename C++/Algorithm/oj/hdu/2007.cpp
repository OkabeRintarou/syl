#include <cstdio>

int main()
{
	int left,right;
	int square,cub;
	while(scanf("%d%d",&left,&right) != EOF){
		if(left > right){
			int t = left;
			left = right;
			right = t;
		}
		square = cub = 0;
		for(int i = left;i <= right;i++){
			if(i & 1){
				cub += i * i * i;
			}else{
				square += i * i;
			}
		}
		printf("%d %d\n",square,cub);
	}
	return 0;
}
