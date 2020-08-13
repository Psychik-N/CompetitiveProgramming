/*
	Problem Link :
	* https://codeforces.com/contest/1284/problem/D
	
	Idea :
	* https://codeforces.com/blog/entry/72804
*/
#pragma GCC optimize("O3")
#pragma GCC target("sse4")

#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define ld long double
#define v vector
#define ALL(x) x.begin(),x.end()
#define rALL(x) x.rbegin(),x.rend()
#define mp make_pair
#define fi first
#define se second
#define endl '\n'

const ll mod = 1e9 + 7;
const ld PI = acos(-1.0);
const ll INF = 1e9;

struct venue {
	ll a, b, idx;
	venue(ll _a = 0, ll _b = 0, ll _idx = 0) : a(_a), b(_b), idx(_idx) {}
	venue(const venue& o) {
		a = o.a;
		b = o.b; 
		idx = o.idx;
	}
};
template<typename T> struct node {
	T mn, mx;
	node(T x = 0, T y = 0) : mn(x), mx(y) {};
};
template<typename T> struct lazyNode {
	T val;
	lazyNode(T x = 0) : val(x) {};
};
template<typename S, typename T> struct SGT {	
	S size;
	vector<node<T>> data;
	vector<lazyNode<T>> lazy;
	vector<bool> isLazy;
	SGT(S sz = 1e6+1) {
		size = sz;
		data.resize(4 * sz);
		lazy.resize(4 * sz);
		isLazy.assign(4 * sz, false);
	}  
	void merge(node<T>& pt, node<T>& lc, node<T>& rc) {
		pt.mn = min(lc.mn, rc.mn);
		pt.mx = max(lc.mx, rc.mx);
	}
	void push(S pt, S lc, S rc, S l, S r) {
		if(!isLazy[pt]) return;
		if(l != r) {
			isLazy[lc] = 1;
			isLazy[rc] = 1;
			lazy[lc].val += lazy[pt].val;
			lazy[rc].val += lazy[pt].val;
		}
		data[pt].mn += lazy[pt].val;
		data[pt].mx += lazy[pt].val;
		lazy[pt].val = 0;
		isLazy[pt] = 0;
	}
	void build(S x, S l, S r, vector<venue>& a) {
		if(l == r) {
			data[x].mn = a[l].b;
			data[x].mx = a[l].a;
		}
		else {
			S mid = ((l + r) >> 1);
			build(2*x, l, mid, a);
			build(2*x+1, mid+1, r, a);
			merge(data[x], data[2*x], data[2*x+1]);
		}
	}
	node<T> query(S x, S l, S r, S qL, S qR) {
		if(isLazy[x]) {
			push(x, 2*x, 2*x+1, l, r);
		}
		if(qL > qR) {
			return node<T>(1e9, 0);
		}
		if((l == qL) && (r == qR)) {
			return data[x];
		}
		S mid = ((l + r) >> 1);
		node<T> res;
		node<T> lc = query(2*x, l, mid, qL, min(mid, qR));
		node<T> rc = query(2*x+1, mid+1, r, max(mid+1, qL), qR);
		merge(res, lc, rc);
		return res;
	}
	void update(S x, S l, S r, S qL, S qR, T val) {
		if(isLazy[x]) {
			push(x, 2*x, 2*x+1, l, r);
		}
		if(qL > qR) {
			return;
		}
		if((l == qL) && (r == qR)) {
			isLazy[x] = 1;
			lazy[x].val = val;
			push(x, 2*x, 2*x+1, l, r);
			return;
		}
		S mid = ((l + r) >> 1);
		update(2*x, l, mid, qL, min(mid, qR), val);
		update(2*x+1, mid+1, r, max(mid+1, qL), qR, val);
		merge(data[x], data[2*x], data[2*x+1]);
	}
};

inline bool compare(venue A, venue B) {
	if(A.a != B.a) return (A.a < B.a);
	else if(A.b != B.b) return (A.b < B.b);
	else return (A.idx < B.idx);
}

bool notSensitive(v<venue> A, v<venue> other) {
	sort(ALL(A), compare);
	ll n = A.size();
	v<venue> B(n);
	for(ll i = 0; i < n; i ++) {
		B[i] = other[A[i].idx];
	}
	SGT<ll, ll> sgt(n);
	sgt.build(1, 0, n-1, B);
	bool res = true;
	for(ll i = 0; i < n; i ++) {
		ll end = upper_bound(ALL(A), venue(A[i].b, 1e9, 1e9), compare) - A.begin() - 1;
		ll start = i;
		node<ll> q = sgt.query(1, 0, n-1, start, end);
		if(!((B[i].a <= q.mn) && (B[i].b >= q.mx))) {
			res = false;
			break;
		}
	}
	return res;
}

int main() {
	ios_base::sync_with_stdio(0);
	cin.tie(0);
	cout.tie(0);
	
	ll n;
	cin >> n;
	v<venue> A(n), B(n);
	for(ll i = 0; i < n; i ++) {
		cin >> A[i].a >> A[i].b;
		cin >> B[i].a >> B[i].b;
		A[i].idx = B[i].idx = i;
	}
	bool pos = (notSensitive(A, B) && notSensitive(B, A));
	if(pos) cout << "YES" << endl;
	else cout << "NO" << endl;
	return 0;
}
