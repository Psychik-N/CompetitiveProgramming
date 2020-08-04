/*
	Problem Link :
	* https://icpcarchive.ecs.baylor.edu/index.php?option=com_onlinejudge&Itemid=8&page=show_problem&problem=5184
	
	Idea :
	* We will solve this problem using DP. dp[i][node] = maximum value of path we can get after selecting i castle in subtree of "node" excluding "node".
	* Now suppose we want to pick i castle in subtree of "node". Now for its child "U" lets suppose we choose j castle (j <= i). Then the path (node --- U).
	- will add to the answer (2 * min(j, K+1-j)). As we can alternately choose castle from subtree of "U" and outside of subtree of "U" and it will be 
	- the optimal way to order the given selected castle.
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
const long long INF = 1e15;
const int maxN = 1e3+1;
const int maxK = 1e2+1;

int N, K;
vector<int> subtree;
vector<vector<pair<int, int>>> graph;
long long dp[maxK][maxN];

void dfs(int node) {
	subtree[node] = 1;
	for(auto i : graph[node]) {
		dfs(i.first);
		subtree[node] += subtree[i.first];
	}
}

long long rec(int req, int node) {
	if(subtree[node]-1 < req) return -1e15;
	if(req == 0) return 0;
	if(subtree[node] == 1) return 0;
	if(dp[req][node] != -1) return dp[req][node];
	int limit = min(subtree[node], K);
	vector<long long> aux(limit+1, -INF);
	aux[0] = 0;
	for(auto i : graph[node]) {
		int to = i.first;
		long long cost = i.second;
		vector<long long> temp(limit+1, -INF);
		for(int j = 0; j <= limit; j ++) {
			temp[j] = aux[j];
			for(int other = 1; other <= j; other ++) {
				long long pathTravel = 2*min(other, K+1-other);
				temp[j] = max(temp[j], aux[j-other] + cost*pathTravel + max(rec(other, to), rec(other-1, to)));
			}
		}
		aux = temp;
	}
	for(int i = 0; i <= limit; i ++) {
		dp[i][node] = aux[i];
	}
	return dp[req][node];
}

void Solve() {
	memset(dp, -1, sizeof(dp));
	cin >> N >> K;
	graph.clear(); graph.resize(N+1);
	subtree.assign(N+1, 0);
	for(int i = 2; i <= N; i ++) {
		int parent, cost;
		cin >> parent >> cost;
		graph[parent].push_back({i, cost});
	}
	dfs(1);
	cout << rec(K, 1) << endl;
}

int main() {
	ios_base::sync_with_stdio(0);
	cin.tie(0);
	cout.tie(0);

	int tt = 1;
	cin >> tt;
	for(int i = 1; i <= tt; i ++) {
		cout << "Case " << i << ": ";
		Solve();
	}
	
	return 0;
}
