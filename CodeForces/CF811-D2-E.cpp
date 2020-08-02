/*
	Problem Link :
	* https://codeforces.com/contest/811/problem/E
	
	Idea :
	* https://codeforces.com/blog/entry/52228
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
const int maxN = 1e5+1;

int n, m, q;
vector<vector<int>> a;

struct node {
	int L[10], R[10];
	int comp;
	node() {
		comp = 0;
	}
	node(const node& other) {
		for(int i = 0; i < 10; i ++) {
			L[i] = other.L[i];
			R[i] = other.R[i];
		}
		comp = other.comp;
	}
};

int parent[10*maxN], sz[10*maxN];
node sgt[4*maxN];

int getParent(int x) {
	return ((parent[x] == x) ? x : parent[x] = getParent(parent[x]));
}

bool merge(int a, int b) {
	int x = getParent(a), y = getParent(b);
	if(x == y) return false;
	if(sz[x] < sz[y]) swap(x, y);
	sz[x] += sz[y];
	parent[y] = x;
	return true;
}

void init(int x) {
	parent[x] = x;
	sz[x] = 1;
}

void unionNode(node &pt, node &lc, node &rc, int column) {
	for(int i = 0; i < n; i ++) {
		init(lc.L[i]); init(lc.R[i]);
		init(rc.L[i]); init(rc.R[i]);
	}
	pt.comp = lc.comp + rc.comp;
	for(int i = 0; i < n; i ++) {
		if(a[i][column] == a[i][column+1]) {
			pt.comp -= merge(lc.R[i], rc.L[i]);	
		}
	}
	for(int i = 0; i < n; i ++) {
		pt.L[i] = getParent(lc.L[i]);
		pt.R[i] = getParent(rc.R[i]);
	}
}

void build(int x, int l, int r) {
	if(l == r) {
		sgt[x].comp = 0;
		for(int i = 0; i < n; i ++) {
			if(i == 0) {
				sgt[x].R[i] = sgt[x].L[i] = i*m + l;
				sgt[x].comp ++;
			}
			else {
				if(a[i][l] != a[i-1][l]) {
					sgt[x].R[i] = sgt[x].L[i] = i*m + l;
					sgt[x].comp ++;
				}
				else {
					sgt[x].R[i] = sgt[x].L[i] = sgt[x].L[i-1];
				}
			}
		}
	}
	else {
		int mid = (l + r) / 2;
		build(2*x, l, mid);
		build(2*x+1, mid+1, r);
		unionNode(sgt[x], sgt[2*x], sgt[2*x+1], mid);
	}
}

node query(int x, int l, int r, int qL, int qR) {
	if((l == qL) && (r == qR)) {
		return sgt[x];
	}
	else {
		int mid = (l + r) / 2;
		if(qR <= mid) return query(2*x, l, mid, qL, qR);
		else if(qL >= mid+1) return query(2*x+1, mid+1, r, qL, qR);
		else {
			node res, left, right;
			left = query(2*x, l, mid, qL, mid);
			right = query(2*x+1, mid+1, r, mid+1, qR);
			unionNode(res, left, right, mid);
			return res;
		}
	}
}

void Solve() {
	cin >> n >> m >> q;
	a.resize(n, vector<int>(m));
	for(int i = 0; i < n; i ++) {
		for(int j = 0; j < m; j ++) {
			cin >> a[i][j];
		}
	}
	build(1, 0, m-1);
	while(q --) {
		int l, r;
		cin >> l >> r;
		node res = query(1, 0, m-1, l-1, r-1);
		cout << res.comp << endl;
	}
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
