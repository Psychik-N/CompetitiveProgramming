/*
	Idea : 
	* https://codeforces.com/blog/entry/64483
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
const int maxN = 2.5e5+1;

int n, m;
vector<int> points(401);
int dp[401][401][401];

void Solve() {
	cin >> n >> m;
	for(int i = 1; i <= n; i ++) {
		cin >> points[i];
	}
	for(int len = 1; len <= n-1; len ++) {
		for(int l = 1; l <= n-1; l ++) {
			int r = l+len;
			if(r > n) continue;
			int opt = l+1;
			for(int part = r-l-1; part >= 0; part --) {
				if(part == 0) {
					dp[l][r][0] = points[r]-points[l];
				}
				else {
					int ans = 2e9;
					for(int cut = opt; cut < r; cut ++) {
						if(r-cut < part) break;
						int res = max(points[cut]-points[l], dp[cut][r][part-1]);
						if(res <= ans) {
							opt = cut;
							ans = res;
						}
						else break;
					}
					dp[l][r][part] = ans;
				}
			}
		}
	}
	long long ans = 0;
	for(int i = 0; i < m; i ++) {
		int s, f, c, r;
		cin >> s >> f >> c >> r;
		r = min(r, f-s-1);
		long long len = dp[s][f][r];
		ans = max(ans, len*c);
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
