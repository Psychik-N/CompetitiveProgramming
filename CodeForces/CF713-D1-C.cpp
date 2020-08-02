/*
	Problem Link :
	* https://codeforces.com/contest/713/problem/C
	
	Idea :
	* https://codeforces.com/blog/entry/47094
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

const ll mod = 1e9 + 7;
const ld PI = acos(-1.0);
const ll maxN = 3e3+1;
const ll INF = 1e18;

long long dp[maxN][maxN];

void Solve() {
	int N;
	cin >> N;
	vector<int> a(N), b(N);
	for(int i = 0; i < N; i ++) {
		cin >> a[i];
		a[i] -= i;
		b[i] = a[i];
	}
	sort(ALL(b));
	for(int i = 0; i < N; i ++) {
		for(int j = 0; j < N; j ++) {
			dp[i][j] = abs(a[i]-b[j]);
			if(i) dp[i][j] += dp[i-1][j];
			if(j) dp[i][j] = min(dp[i][j], dp[i][j-1]);
		}
	}
	long long ans = INF;
	for(int i = 0; i < N; i ++) {
		ans = min(ans, dp[N-1][i]);
	}
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
