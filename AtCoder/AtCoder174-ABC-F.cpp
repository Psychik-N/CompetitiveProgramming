/*
	Idea :
	* Solve queries offline.
	* Only add the leftmost occurance of the color to BIT. Now suppose if we move the left pointer of query forward, we will then remove the current
	- occurance and add the next smallest occurance to the BIT.
	* To answer the query we will just do range sum.
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
const int maxN = 5e5+1;

struct BIT {
	using T = int;
	vector<T> arr;
	T total;
	BIT(T s = 1e6) {
		total = 0;
		arr.assign(s+1 , 0);
	}
	T sum(T pos) {
		T ans = 0;
		while(pos > 0) {
			ans += arr[pos];
			pos = pos - (pos & (-pos));
		}
		return ans;
	}
	void update(T pos, T val) {
		while((pos > 0) && (pos < arr.size())) {
			arr[pos] += val;
			pos = pos + (pos & (-pos));
		}
	}
	T rQuery(T l, T r) {
		return sum(r) - sum(l-1);
	}
};

void Solve() {
	int N, Q;
	cin >> N >> Q;
	vector<int> ptr(maxN, 0);
	vector<int> a(N+1);
	vector<vector<int>> color(maxN);
	BIT dis(maxN);
	for(int i = 1; i <= N; i ++) {
		int in;
		cin >> in;
		a[i] = in;
		if(color[in].size() == 0) dis.update(i, 1);
		color[in].push_back(i);
	}
	vector<int> ans(Q);
	vector<tuple<int, int, int>> query(Q);
	for(int i = 0; i < Q; i ++) {
		int L, R;
		cin >> L >> R;
		query[i] = make_tuple(L, R, i);
	}
	sort(query.begin(), query.end());
	int id = 1;
	for(int i = 0; i < Q; i ++) {
		while(id < get<0>(query[i])) {
			int cur = a[id];
			dis.update(color[cur][ptr[cur]], -1);
			ptr[cur] ++;
			if(color[cur].size() >= ptr[cur]+1) {
				dis.update(color[cur][ptr[cur]], 1);
			}
			id ++;
		}
		ans[get<2>(query[i])] = dis.rQuery(get<0>(query[i]), get<1>(query[i]));
	}
	for(auto i : ans) cout << i << endl;
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
