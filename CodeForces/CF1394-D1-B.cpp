/*
	Problem Link :
	* https://codeforces.com/contest/1394/problem/B
	
	Idea :
	* Consider the graph for a given tuple of (c1, c2, c3, ..., ck). Now you will observe that after you remove all the egdes which is non-visitable from
	- the node you will get a graph with every node with 1 out-going edge(1 out-degree).
	
	* Now to fulfill the condition in the question each node should have 1 in-degree. By this we will have a graph with bunch of simple cycles.
	
	* So now make a in-degree vector for each (1, 1), (2, 1), (2, 2), ... where first pair is index in the tuple and second is ci.
	
	* Now problem is reduced where you have to select one vector from (1, 1), one from (2, 1), (2, 2), one from (3, 1), (3, 2), (3, 3) ... so on.
	- and add those vector and at each position you should get 1.
	
	* Give some ID to each pair.
	
	* Compute a badPair matrix, badPair[i][j] is one when there is an intersection in vector when pair with ID i, and pair with ID j is added. Here intersection
	- means there is node whose in-degree will become more than 1 if we take both these pair.
	
	* To count number of correct combination we can do brute force, "45" computation will be required to check each combination(using badPair matrix) and 
	- there are total 9! combination.
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
const int maxN = 2e5+1;

int n, m, k, ans = 0, permut[10], in[45][maxN], id[46][46], totID = 0;
vector<vector<pair<int, int>>> graph;
bool badPair[45][45];

void dfs(int depth) {
	if(depth > k) {
		ans ++;
		return;
	}
	for(int i = 1; i <= depth; i ++) {
		int cur = id[depth][i];
		if(badPair[cur][cur]) continue;
		bool ispos = true;
		for(int j = 1; j < depth; j ++) {
			if(badPair[cur][permut[j]]) ispos = false;
		}
		if(ispos) {
			permut[depth] = cur;
			dfs(depth+1);
		}
	}
}

void Solve() {
	cin >> n >> m >> k;
	graph.resize(n+1);
	for(int i = 1; i <= m; i ++) {
		int a, b, w;
		cin >> a >> b >> w;
		graph[a].push_back({w, b});
	}
	for(int i = 1; i <= n; i ++) {
		sort(ALL(graph[i]));
	}
	for(int i = 1; i <= k; i ++) {
		for(int j = 1; j <= i; j ++) {
			id[i][j] = totID ++;
		}
	}
	for(int i = 1; i <= n; i ++) {
		for(int j = 0; j < graph[i].size(); j ++) {
			in[id[graph[i].size()][j+1]][graph[i][j].second] ++;
		}
	}
	for(int i = 1; i <= n; i ++) {
		for(int j = 0; j < totID; j ++) {
			if(in[j][i] > 1) badPair[j][j] = 1;
			for(int k = 0; k < totID; k ++) {
				if(j == k) continue;
				if(in[j][i]+in[k][i] > 1) {
					badPair[j][k] = 1;
				}
			}
		}
	}
	dfs(1);
	cout << ans << endl;
} 

int main() {
	ios_base::sync_with_stdio(0);
	cin.tie(0);
	cout.tie(0);

	int tt = 1;	
	while(tt --) {
		Solve();
	}
	
	return 0;
}
