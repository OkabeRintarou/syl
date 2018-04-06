#include <cstdio>
#include <numeric>
#include <algorithm>

using namespace std;

int arr[101];

int main()
{
	int v = 2;
	for(int i = 1;i < 101;i++){
		arr[i] = v;
		v += 2;
	}
	int n,m,count,flag;
	while(scanf("%d%d",&n,&m) != EOF){
		int i;
		count = flag = 0;
		for(i = 1;i <= n;){
			int s,end,v;
			if(i + m > n){
				s = accumulate(arr + i,arr + n + 1,0);
				v = (int)(s * 1.0 / (n - i + 1));
				flag = 1;
			}else{
				s = accumulate(arr + i,arr + i + m,0);
				v = (int)(s * 1.0 / m);
				i += m;
			}
			if(!count){
				printf("%d",v);
				++count;
			}else{
				printf(" %d",v);
			}
			if(flag){
				printf("\n");
				break;
			}
		}
	}
	return 0;
}
