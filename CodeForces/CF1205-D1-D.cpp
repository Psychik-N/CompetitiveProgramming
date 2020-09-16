/*
	Problem Link :
	* https://codeforces.com/contest/1205/problem/D
	
	Idea :
	* https://codeforces.com/blog/entry/69158
*/
#pragma GCC target ("avx2")
#pragma GCC optimization ("O3")
#pragma GCC optimization ("unroll-loops")

#include <bits/stdc++.h>
using namespace std;

#define v vector
#define ALL(x) x.begin(),x.end()
#define rALL(x) x.rbegin(),x.rend()
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
const int maxN = 2e5+1;

int N, Centroid = -1, smalldone = 0, largedone = 0;
vector<vector<int>> g(int(1001));
vector<int> subtree(int(1001), 0);

int getCentroid(int node, int par) {
	if(Centroid != -1) {
		return 0;
	}
	int sub = 1;
	for(auto i : g[node]) {
		if(i == par) continue;
		sub += getCentroid(i, node);
	}
	if((Centroid == -1) && (sub > N/2)) {
		Centroid = node;
	}
	return sub;
}

void assignSub(int node, int par) {
	subtree[node] = 1;
	for(auto i : g[node]) {
		if(i == par) continue;
		assignSub(i, node);
		subtree[node] += subtree[i];
	}
}

bool comparator(int a, int b) {
	return (subtree[a] < subtree[b]);
}

void assignWeight(int node, int parent, int sum, int weight, bool isSmall) {
	int req;
	if(isSmall) {
		smalldone ++;
		req = smalldone;
	}
	else {
		largedone ++;
		req = largedone;
	}
	cout << parent << " " << node << " " << (req-sum)*weight << "\n";
	for(auto i : g[node]) {
		if(i == parent) continue;
		assignWeight(i, node, req, weight, isSmall);
	}
}

void Solve() {
	cin >> N;
	for(int i = 0; i < N-1; i ++) {
		int a, b;
		cin >> a >> b;
		g[a].push_back(b);
		g[b].push_back(a);
	}
	getCentroid(1, -1);
	assignSub(Centroid, -1);
	sort(ALL(g[Centroid]), comparator);
	int req = (2*N*N)/9;
	for(auto i : g[Centroid]) {
		int Fi = smalldone, Se = N-1-smalldone;
		if(Fi*Se + Fi + Se < req) {
			assignWeight(i, Centroid, 0, 1, 1);
		}
		else {
			assignWeight(i, Centroid, 0, smalldone+1, 0);
		}
	}
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
