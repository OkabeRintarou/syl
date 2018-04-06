#include <cstdio>

int main()
{
	int n,total;
	char c;
	scanf("%d\n",&n);
	for(int i = 0;i < n;i++){
		total = 0;
		for(;;){
			scanf("%c",&c);
			if(c == '\n')break;
			if(c >= '0' && c <= '9'){
				++total;
			}
		}
		printf("%d\n",total);
	}
	return 0;
}
