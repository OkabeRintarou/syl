#include <cstdio>
#include <climits>
#include <algorithm>

using namespace std;

int arr[105];

int main()
{
	int n;
	while(scanf("%d",&n) != EOF){
		if(n == 0)break;
		int m = INT_MAX,index;
		for(int i = 0;i < n;i++){
			scanf("%d",&arr[i]);
			if(arr[i] < m){
				m = arr[i];
				index = i;
			}
		}
		swap(arr[0],arr[index]);
		for(int i = 0;i < n;i++){
			if(i){
				printf(" %d",arr[i]);
			}else{
				printf("%d",arr[i]);
			}
		}
		printf("\n");
	}
	return 0;
}
