#include <iostream>
#include <vector>
#include <queue>
#include <list>

using namespace std;

struct Node
{
	enum Color{ WHITE, GRAY, BLACK };
	Color color;
	int d;
	int id;
};

int Bfs(vector<Node> & nodes, vector<list<int> > & adj, int A, int B)
{
	nodes[A - 1].color = Node::GRAY;
	nodes[A - 1].d = 0;

	queue<Node> Q;
	Q.push(nodes[A - 1]);

	while (!Q.empty()){
		Node u = Q.front();
		Q.pop();
		int id = u.id;
		
		list<int> & ls = adj[id];
		for (list<int>::iterator it = ls.begin(); it != ls.end(); it++){
			
			if (nodes[*it - 1].color == Node::WHITE){
				nodes[*it - 1].color = Node::GRAY;
				nodes[*it - 1].d = u.d + 1;
				if (*it == B){
					return nodes[*it - 1].d;
				}
				Q.push(nodes[*it - 1]);
			}
		}
		u.color = Node::BLACK;
	}

	return 0;
}

int main()
{
	int N, A, B;
	cin >> N >> A >> B;
	vector<int> K;
	K.resize(N);
	int i = 0;

	while (cin >> K[i] && K[i] != 0){
		i++;
	}

	vector<list<int> > adj(N);
	for (int i = 1; i <= N; i++){
		if (i + K[i - 1] <= N){
			adj[i - 1].push_back(i + K[i - 1]);
		}
		if (i - K[i - 1] >= 1){
			adj[i - 1].push_back(i - K[i - 1]);
		}
	}


	vector<Node> nodes(N);
	for (int i = 0; i < N; i++){
		nodes[i].color = Node::WHITE;
		nodes[i].d = 0;
		nodes[i].id = i;
	}

	int ret = Bfs(nodes, adj, A, B);
	if (ret == 0){
		cout << -1;
	}
	else{
		cout << ret;
	}
	return 0;
}