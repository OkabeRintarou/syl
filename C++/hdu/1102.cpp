#include <cstdio>
#include <queue>

using namespace std;

static const int N = 105;
int parent[N];
int rank_[N];

struct Edge{
	int src;
	int dst;
	int weight;
	Edge(int s,int d,int w):src(s),dst(d),weight(w){}
};

bool operator<(Edge lhs,Edge rhs){
	return lhs.weight > rhs.weight;
}

void init(int size)
{
	for(int i = 0;i < size;i++){
		parent[i] = i;
		rank_[i] = 0;
	}
}


int findSet(int x)
{
	if(x != parent[x]){
		parent[x] = findSet(parent[x]);
	}
	return parent[x];
}

void union_(int x,int y)
{
	int px,py;
	px = findSet(x);
	py = findSet(y);
	if(rank_[px] > rank_[py]){
		parent[py] = px;
	}else{
		parent[px] = py;
		if(rank_[px] == rank_[py]){
			rank_[py]++;
		}
	}
}

priority_queue<Edge> pq;

int main()
{
	int n;
	while(scanf("%d",&n) != EOF){
		init(n);
		pq = priority_queue<Edge>();
		int w;
		for(int i = 0;i < n;i++){
			for(int j = 0;j < n;j++){
				scanf("%d",&w);
				if(i < j){
					pq.push(Edge(i,j,w));
				}
			}
		}
		int Q;
		scanf("%d",&Q);
		for(int i = 0;i < Q;i++){
			int a,b;
			scanf("%d %d",&a,&b);
			union_(--a,--b);
		}

		int total = 0;
		while(!pq.empty()){
			Edge t = pq.top();
			pq.pop();
			if(findSet(t.src) != findSet(t.dst)){
				total += t.weight;
				union_(t.src,t.dst);
			}
		}
		printf("%d\n",total);
	}
	return 0;
}
