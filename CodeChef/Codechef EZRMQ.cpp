/*
	Problem Link :
	* https://www.codechef.com/COOK119A/problems/EZRMQ
	
	Idea :
	* https://discuss.codechef.com/t/ezrmq-editorial/69458
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

const long long mod = 1e9+7;
const long double PI = acos(-1.0);
const long long INF = 1e18;
const int maxN = 5e3+1;

int N, root, subtree[maxN][2];
long long a[maxN], dp[maxN][maxN];
vector<vector<int>> graph(maxN, vector<int>(2, -1));

int buildGraph(int L, int R) {
	if(L > R) return -1;
	int maxEle = 0, id = -1;
	for(int i = L; i <= R; i ++) {
		if(maxEle < a[i]) {
			maxEle = a[i];
			id = i;
		}
	}
	graph[id][0] = buildGraph(L, id-1);
	if(graph[id][0] != -1) {
		int node = graph[id][0];
		subtree[id][0] = subtree[node][0] + subtree[node][1] + 1;
	}
	else subtree[id][0] = 0;
	graph[id][1] = buildGraph(id+1, R);
	if(graph[id][1] != -1) {
		int node = graph[id][1];
		subtree[id][1] = subtree[node][0] + subtree[node][1] + 1;
	}
	else subtree[id][1] = 0;
	return id;
}

void rec(int node) {
	if(subtree[node][0] + subtree[node][1] == 0) {
		dp[node][0] = 0;
		dp[node][1] = a[node];
		return;
	} 
	if(subtree[node][0]) rec(graph[node][0]);
	if(subtree[node][1]) rec(graph[node][1]);
	int L = graph[node][0];
	int R = graph[node][1];
	int maxSize = subtree[node][0]+subtree[node][1]+1;
	for(int i = 1; i <= maxSize; i ++) {
		dp[node][i] = INF;
	}
	if((L != -1) && (R == -1)) {
		for(int i = 0; i <= subtree[node][0]; i ++) {
			dp[node][i] = min(dp[node][i], dp[L][i]);
			dp[node][i+1] = min(dp[node][i+1], dp[L][i]+((i+1)*a[node]));
		}
	}
	else if((L == -1) && (R != -1)) {
		for(int i = 0; i <= subtree[node][1]; i ++) {
			dp[node][i] = min(dp[node][i], dp[R][i]);
			dp[node][i+1] = min(dp[node][i+1], dp[R][i]+((i+1)*a[node]));
		}
	}
	else {
		for(int i = 0; i <= subtree[node][0]; i ++) {
			for(int j = 0; j <= subtree[node][1]; j ++) {
				dp[node][i+j] = min(dp[node][i+j], dp[L][i] + dp[R][j] + (i*j*a[node]));
				dp[node][i+j+1] = min(dp[node][i+j+1], dp[L][i] + dp[R][j] + ((i+1)*(j+1)*a[node]));
			}
		}
	}
}

void Solve() {
	int K;
	cin >> N >> K;
	for(int i = 1; i <= N; i ++) {
		cin >> a[i]; 
	}
	root = buildGraph(1, N);
	rec(root);
	cout << dp[root][K] << endl;
}

int main() {
	ios_base::sync_with_stdio(0);
	cin.tie(0);
	cout.tie(0);

	int tt = 1;
	cin >> tt;
	while(tt --) {
		Solve();
	}
	
	return 0;
}
