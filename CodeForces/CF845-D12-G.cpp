/*
	Problem Link :
	* https://codeforces.com/contest/845/problem/G
	
	Idea : 
	* Learn about Cycle Basis.
	* Application of gauss.
*/
#pragma GCC target ("avx2")
#pragma GCC optimization ("O3")
#pragma GCC optimization ("unroll-loops")

#include <bits/stdc++.h>
using namespace std;

#define v vector
#define ALL(x) x.begin(),x.end()
#define rALL(x) x.rbegin(),x.rend()
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
const int maxN = 1e5+1;

int N, M;
vector<bool> sel, vis(maxN, false);
vector<int> graph[maxN];
vector<tuple<int, int, int>> edge;
vector<int> basis;
vector<int> Distance(maxN);
vector<int> fundamentalBasis(30, -1);

void getSpanningTree(int node, int d) {
	vis[node] = 1;
	Distance[node] = d;
	for(auto i : graph[node]) {
		int to, w;
		if(get<0>(edge[i]) == node) to = get<1>(edge[i]);
		else to = get<0>(edge[i]);
		if(vis[to]) continue;
		w = get<2>(edge[i]);
		sel[i] = 1;
		getSpanningTree(to, d^w);
	}
}

void getCycleBasis() {
	for(int i = 0; i < M; i ++) {
		if(sel[i]) continue;
		int a = get<0>(edge[i]);
		int b = get<1>(edge[i]);
		int w = get<2>(edge[i]);
		basis.push_back(Distance[a]^Distance[b]^w);
	}
}

void gauss() {
	int cur = 0;
	for(int i = 29; i >= 0; i --) {
		int ptr = -1;
		for(int j = cur; j < basis.size(); j ++) {
			if(basis[j] & (1 << i)) {
				ptr = j;
				break;
			}
		}
		if(ptr == -1) {
			continue;
		}
		swap(basis[ptr], basis[cur]);
		fundamentalBasis[i] = cur;
		for(int j = 0; j < basis.size(); j ++) {
			if(j == cur) continue;
			if(basis[j] & (1 << i)) basis[j] ^= basis[cur];
		}
		cur ++;
	}
}

void Solve() {
	cin >> N >> M;
	for(int i = 0; i < M; i ++) {
		int a, b, w;
		cin >> a >> b >> w;
		graph[a].push_back(i);
		graph[b].push_back(i);
		edge.push_back(make_tuple(a, b, w));
		sel.push_back(0);
	}
	getSpanningTree(1, 0);
	getCycleBasis();
	sort(rALL(basis));
	gauss();
	int ans = Distance[N];
	dbg(ans);
	for(int i = 29; i >= 0; i --) {
		if((ans & (1 << i)) && ((fundamentalBasis[i] != -1))) {
			ans ^= basis[fundamentalBasis[i]];
		}
	}	
	cout << ans << endl;
}

int main() {
	ios_base::sync_with_stdio(0);
	cin.tie(0);
	cout.tie(0);

	int tt = 1;
	for(int i = 1; i <= tt; i ++) {
		Solve();
	}
	
	return 0;
}
