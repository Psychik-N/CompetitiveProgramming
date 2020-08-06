/*
	Problem Link : 
	* https://codeforces.com/contest/1399/problem/F
	
	Idea :
	* https://codeforces.com/blog/entry/81082
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
const int maxN = 2e5+1;

int N, SZ;
vector<pair<int, int>> seg;
map<int, int> ID;
vector<int> CC;
vector<vector<int>> dp;
vector<vector<int>> rightEnd;

int rec(int L, int R) {
	if(R < L) return 0;
	if(dp[L][R] != -1) return dp[L][R];
	if(rightEnd[R].size() == 0) return dp[L][R] = rec(L, R-1);
	int ans = 0, same = 0;
	int start = lower_bound(rightEnd[R].begin(), rightEnd[R].end(), L) - rightEnd[R].begin();
	if((rightEnd[R][start] == L) && (start != rightEnd[R].size())) {
		ans ++;
		same ++;
		start ++;
	}
	for(int i = start; i < rightEnd[R].size(); i ++) {
		int LL = rightEnd[R][i];
		ans = max(ans, rec(L, LL-1) + rec(LL, R) + same);
	}
	ans = max(ans, rec(L, R-1)+same);
	dp[L][R] = ans;
	return ans;
}

void Solve() {
	cin >> N;
	CC.clear();
	seg.clear();
	for(int i = 0; i < N; i ++) {
		int L, R;
		cin >> L >> R;
		seg.push_back({L, R});
		CC.push_back(L);
		CC.push_back(R);
	}
	sort(CC.begin(), CC.end());
	CC.resize(unique(CC.begin(), CC.end())-CC.begin());
	SZ = CC.size();
	for(int i = 0; i < SZ; i ++) {
		ID[CC[i]] = i;
	}
	dp.assign(SZ, vector<int>(SZ, -1));
	rightEnd.assign(SZ, vector<int>());
	for(auto i : seg) {
		int L = i.first, R = i.second;
		rightEnd[ID[R]].push_back(ID[L]);
	}
	for(int i = 0; i < SZ; i ++) {
		sort(rightEnd[i].begin(), rightEnd[i].end());
	}
	cout << rec(0, SZ-1) << endl;
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
