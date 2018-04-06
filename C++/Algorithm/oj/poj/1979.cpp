#include <cstdio>
#include <cassert>
#include <queue>
#include <utility>

using namespace std;

static const int W = 23;
static const int H = 23;

char data[H][W];
int dir[4][2]={{0,1},{0,-1},{1,0},{-1,0}};

int bfs(int x,int y,int w,int h)
{
	queue<pair<int,int> > q;
	int i,j;
	int count = 0;

	q.push(make_pair(x,y));
	
	while(!q.empty()){
		i = q.front().first;
		j = q.front().second;
		q.pop();
		for(int k = 0;k < 4;k++){
			int newx = i + dir[k][0];
			int newy = j + dir[k][1];
			if(newx >= 0 && newx < h && newy >= 0 && newy < w && data[newx][newy] == '.'){
				count++;
				data[newx][newy] = -1;
				q.push(make_pair(newx,newy));
			}
		}
	}

	return count + 1;
}

int main()
{
	int w,h;
	int srcx,srcy;

	while(scanf("%d%d",&w,&h) != EOF && w && h){
		srcx = srcy = -1;
		for(int i = 0;i < h;i++){
			scanf("%s",(char*)&data[i]);
			for(int j = 0;j < w && srcx < 0;j++){
				if(data[i][j] == '@'){
					srcx = i;srcy = j;break;
				}
			}
		}

		assert(srcx >= 0 && srcy >= 0);
		printf("%d\n",bfs(srcx,srcy,w,h));

	}
	return 0;
}
