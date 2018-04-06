#include <cstdio>

int main()
{
	int n,m;
	while(scanf("%d%d",&n,&m) != EOF){
		if(n == 0 && m == 0)break;
		int v;
		int flag = 1;
		for(int i = 0;i < n;i++){
			scanf("%d",&v);
			if(flag && m < v){
				if(i == 0){
					printf("%d %d",m,v);
				}else{
					printf(" %d %d",m,v);
				}
				flag = 0;
			}else{
				if(i == 0)printf("%d",v);
				else printf(" %d",v);
			}
		}
		printf("\n");
	}
	return 0;
}
