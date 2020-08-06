/*
	Problem Link :
	* https://onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&page=show_problem&problem=4110
	
	Idea :
	* Read about Biconnected Component and Bipartite Graph.
	
	* Form the graph of edges which is not present and find its BCC.
	
	* Now for each BCC, if the BCC is Bipartite it means the cycle basis of the bcc will be of even length and there is no way to put any node in the graph
	- in odd cycle.
	
	* If the BCC is not bipartite graph, then we are sure to find at least one odd cycle(because the graph is BCC). So for each node either it will be either
	- in odd cycle and if it is in even cycle we can make it in odd cycle using linear combination of basis.
*/
#pragma GCC target ("avx2")
#pragma GCC optimization ("O3")
#pragma GCC optimization ("unroll-loops")

#include <bits/stdc++.h>
using namespace std;

#define v vector
#define ALL(x) x.begin(),x.end()
#define rALL(x) x.rbegin(),x.rend()
#define endl '\n'
#define F first
#define S second
#define DEBUG

template<typename T> void DBG(const char* name, T&& H) {
	cerr << name << " = " << H << ')' << endl;
}

template<typename T, typename... Args> void DBG(const char* names, T&& H, Args&&... args) {
	const char* NEXT = strchr(names+1, ',');
	cerr.write(names, NEXT-names) << " = " << H << " |";
	DBG(NEXT+1, args...);
}

#ifdef DEBUG
	#define dbg(...) cerr << __FUNCTION__ << ' ' <<__LINE__ << " : (", DBG(#__VA_ARGS__, __VA_ARGS__)
#else
	#define dbg(...)
#endif

using ll = long long;
using ld = long double;

const long long mod = 998244353;
const long double PI = acos(-1.0);
const long long INF = 1e18;
const int maxN = 1e3+1;

vector<vector<int>> graph;

struct BCC {
	int _N;
	int bccCount, timer;
	vector<int> tin, low, bccColor;
	vector<vector<int>> bcc;
	stack<pair<int, int>> edge;
	void init(int N) {
		_N = N;
		tin.assign(N+1, 0);
		low.assign(N+1, 0);
		bccColor.assign(N+1, 0);
		bcc.clear();
		bcc.resize(N+1);
		bccCount = timer = 0;
	}
	void dfs(int node, int parent) {
		tin[node] = low[node] = timer ++;
		for(auto to : graph[node]) {
			if(!tin[to]) {
				edge.push({node, to});
				dfs(to, node);
				low[node] = min(low[node], low[to]);
				if(low[to] >= tin[node]) {
					bccCount ++;
					while(true) {
						int a = edge.top().first;
						int b = edge.top().second;
						edge.pop();
						if(bccColor[a] != bccCount) {
							bcc[bccCount].push_back(a);
							bccColor[a] = bccCount;
						}
						if(bccColor[b] != bccCount) {
							bcc[bccCount].push_back(b);
							bccColor[b] = bccCount;
						}
						if((a == node) && (b == to)) {
							break;
						}
					}
				}
			}
			if((to != parent) && (tin[to] < tin[node])) {
				edge.push({node, to});
				low[node] = min(low[node], tin[to]);
			}
		}
	}
	void processBCC() {
		for(int i = 1; i <= _N; i ++) {
			if(tin[i] == 0) {
				timer = 1;
				dfs(i, -1);
			}
		}
	}
};


int N, M;
BCC getCom;
vector<int> color;

bool isBipartite(int node) {
	for(auto to : graph[node]) {
		if(getCom.bccColor[to] != getCom.bccColor[node]) continue;
		if(color[to] == 0) {
			color[to] = 3-color[node];
			if(!isBipartite(to)) return false;
		}
		if(color[node] == color[to]) return false;
	}
	return true;
}

void Solve() {
	graph.clear();
	graph.resize(N+1);
	vector<vector<bool>> edge(N+1, vector<bool>(N+1, false));
	for(int i = 1; i <= M; i ++) {
		int a, b;
		cin >> a >> b;
		edge[a][b] = edge[b][a] = 1;
	}
	for(int i = 1; i <= N; i ++) {
		for(int j = i+1; j <= N; j ++) {
			if(edge[i][j]) continue;
			graph[i].push_back(j);
			graph[j].push_back(i);
		}
	}
	getCom.init(N);
	getCom.processBCC();
	int toRemove = N;
	vector<bool> inOddCycle(N+1, false);
	for(int i = 1; i <= getCom.bccCount; i ++) {
		color.assign(N+1, 0);
		int start = getCom.bcc[i][0];
		for(auto node : getCom.bcc[i]) {
			getCom.bccColor[node] = i;
		}
		color[start] = 1;
		if(!isBipartite(start)) {
			for(auto node : getCom.bcc[i]) {
				inOddCycle[node] = 1;
			}
		}
	}
	for(int i = 1; i <= N; i ++) {
		if(inOddCycle[i]) toRemove --;
	}
	cout << toRemove << endl;
}

int main() {
	ios_base::sync_with_stdio(0);
	cin.tie(0);
	cout.tie(0);
	
	while(true) {
		cin >> N >> M;
		if(N == 0) break;
		Solve();
	}
	
	return 0;
}
