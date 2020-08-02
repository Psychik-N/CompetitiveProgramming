/*
	Problem Link :
	* https://www.spoj.com/problems/INCDSEQ/
	
	Idea :
	* As the array length N <= 10000, So it can have at max N distinct element. So use cordinate compression to convert the element magnitude between 1 to N.
	* Now for each element compute the number of distinct sequence of length 1, 2, ..., K, using dp.
	* Let compressed weight of a[i] = x. dp[x][i] = dp[1][i-1] + dp[2][i-1] + ... + dp[x-1][i-1]. This quantity can be calculated efficiently using
	- Binary Index Tree or Segment Tree.
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

const long long mod = 5e6;
const long double PI = acos(-1.0);
const long long INF = 1e18;
const int maxN = 1e8+1;

struct BIT {
	using T = int;
	vector<T> arr;
	T total;
	BIT() {
		total = 0;
		arr.assign(10001 , 0);
	}
	T sum(T pos) {
		T ans = 0;
		while(pos > 0) {
			ans += arr[pos];
			if(ans > mod) ans -= mod;
			pos = pos - (pos & (-pos));
		}
		return ans;
	}
	void update(T pos, T val) {
		while((pos > 0) && (pos < arr.size())) {
			arr[pos] += val;
			if(arr[pos] > mod) arr[pos] -= mod;
			pos = pos + (pos & (-pos));
		}
	}
	T rQuery(T l, T r) {
		if(r < l) return 0;
		T value = sum(r)-sum(l-1);
		if(value < 0) value += mod;
		return value;
	}
};

void Solve() {
	int N, K;
	cin >> N >> K;
	vector<int> a(N), b;
	for(int i = 0; i < N; i ++) {
		cin >> a[i];
	}
	b = a;
	sort(ALL(b));
	int ID = 1;
	map<int, int> CC; // Cordinate Compression
	for(auto i : b) {
		if(CC.count(i)) continue;
		CC[i] = ID;
		ID ++;
	}

	BIT dp[K];
	for(int i = 0; i < N; i ++) {
		int ele = CC[a[i]];
		for(int j = 1; j <= K; j ++) {
			int idx = j-1;
			int prev = dp[idx].rQuery(ele, ele);
			prev = mod-prev;
			dp[idx].update(ele, prev);
			if(j == 1) {
				dp[idx].update(ele, 1);
			}
			else {
				int comb = dp[idx-1].rQuery(1, ele-1);
				dp[idx].update(ele, comb);
			}
		}
	}
	int ans = dp[K-1].rQuery(1, 10000);
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
