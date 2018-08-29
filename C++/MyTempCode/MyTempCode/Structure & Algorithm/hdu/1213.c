#include <stdio.h>
int p[1024];


int Find(int x)
{
	if (x == p[x]){
		return x;	
	}
	return (p[x] =Find(p[x]));
}

void Union(int x, int y)
{
	int xf = Find(x);
	int yf = Find(y);
	p[xf] = yf;
}

int main()
{
	int T;
	int N, M;
	int i;
	int x, y;

	scanf("%d", &T);
	while (T--){
		scanf("%d%d", &N, &M);
		for (i = 1; i <= N; i++) p[i] = i;
		for (i = 1; i <= M; i++){
			scanf("%d%d", &x, &y);
			Union(x, y);
		}
		int num = 0;
		for (i = 1; i <= N; i++){
			if (i == p[i]){
				num++;
			}
		}
		printf("%d\n", num);
	}
	
}