#include <cstdio>
#include <algorithm>

using namespace std;

int main()
{	
	char a[3];
	char tmp;
	while(scanf("%s",a) != EOF){
		sort(a,a + 3);
		printf("%c %c %c\n",a[0],a[1],a[2]);
	}
}
