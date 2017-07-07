#include <cstdio>
#include <cstring>

using namespace std;

static const int N = 100005;
int parent[N];
int rank_[N];
int totalNodes;
int unionCount;
bool hasCircle;

void makeSet(int x)
{
	parent[x] = x;
	rank_[x] = 0;
}

int findSet(int x){return x != parent[x] ? parent[x] = findSet(parent[x]) : parent[x];}

void union_(int x,int y)
{
	int px = findSet(x);
	int py = findSet(y);

	hasCircle = px == py;
	unionCount++;

	if(rank_[px] > rank_[py]){
		parent[py] = px;
	}else{
		parent[px] = py;
		if(rank_[px] == rank_[py]){
			rank_[py]++;
		}
	}
}

void init()
{
	memset(parent,0,sizeof(parent));
	memset(rank_,0,sizeof(rank_));
	totalNodes = 0;
	unionCount = 0;
	hasCircle = false;
}

int main()
{
	int a,b;
	init();

	while(scanf("%d %d",&a,&b) != EOF){
		if(a == -1 && b == -1)break;
		if(a == 0 && b == 0){
			if(totalNodes == 0){
				printf("Yes\n");
			}else if(hasCircle || unionCount != totalNodes - 1){
				printf("No\n");
			}else{
				printf("Yes\n");
			}
			init();
			continue;
		}
		if(parent[a] == 0){
			makeSet(a);
			totalNodes++;
		}
		if(parent[b] == 0){
			makeSet(b);
			totalNodes++;
		}

		union_(a,b);
	}
	return 0;
}
