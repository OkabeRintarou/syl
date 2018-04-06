#include <cstdio>

int main()
{
	int a[60]={0,1,2,3,4};  
    for(int i=5; i<56; i++)  
        a[i]=a[i-1]+a[i-3];  
    int n;  
    while(scanf("%d",&n)!=EOF,n)  
    {  
        printf("%d\n",a[n]);  
    }  
  
	return 0;
}
