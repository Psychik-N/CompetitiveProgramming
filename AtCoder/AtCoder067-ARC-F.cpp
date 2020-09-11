/*
	Problem Link : 
	* https://atcoder.jp/contests/arc067/tasks/arc067_d
	
	Idea : 
	* It can be solved using Divide and Conquer DP Optimization for this problem.
	
	* Basic idea is that if we fix leftmost(L) and the rightmost(R) hotel then we can take the maximum of M values out of the given range and
	- subtract the travel distance which is constant for given L and R. So maximum value out of all pairs of L and R.
	
	* Suppose for a L the maximum value we get at index i, then for L+1 the optimal value will lie between [i, N], and this observation allows us 
	- to use Divide and Conquer DP Optimization.
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

struct SparseTable {
	using S = int;
	using T = long long;
	S n, k, m;
	vector<vector<vector<T>>> table;
	void init(S N, S M) {
		n = N;
		m = M;
		k = (S)(log2(n)) + 1;
		table.assign(k+1, vector<vector<T>> (m, vector<T>(n, 0)));
	}
	inline T func(T a, T b) {
		return max(a, b);
	}
	void build(vector<vector<T>> &arr) {
		for(int i = 0; i < n; i ++) {
			for(int j = 0; j < m; j ++) {
				table[0][j][i] = arr[j][i];
			}
		}
		for(S j = 1; j <= k; j ++) {
			for(S l = 0; l < m; l ++) {
				for(S i = 0; i + (1<<j) <= n; i ++) {
					table[j][l][i] = func(table[j-1][l][i], table[j-1][l][i + (1 << (j-1))]);
				}
			}
		}
	}
	T query(S left, S right) {
		S length = (right - left + 1);
		S K = (S)(log2(length));
		T ans = 0;
		for(int i = 0; i < m; i ++) {
			ans += func(table[K][i][left], table[K][i][right + 1 - (1 << K)]);
		}
		return ans;
	}
};

int N, M;
vector<long long> A(int(5e3), 0);
vector<vector<long long>> mat(200, vector<long long>(int(5e3)));
SparseTable st;

long long CostFunction(long long L, long long R) {
	long long happy = st.query(L, R);
	happy -= A[R] - A[L];
	return happy;
} 

long long DivideAndConquerDP(int L, int R, int optL, int optR) {
	if(L > R) return -1e18;
	int M = (L+R)/2, nextOpt = -1;
	long long best = -1e18;
	for(int i = max(M, optL); i <= optR; i ++) {
		long long val = CostFunction(M, i);
		if(val >= best) {
			best = val;
			nextOpt = i;
		}
	}
	best = max({best, DivideAndConquerDP(L, M-1, optL, nextOpt), DivideAndConquerDP(M+1, R, nextOpt, optR)});
	return best;
}

void Solve() {
	cin >> N >> M;
	for(int i = 1; i < N; i ++) {
		cin >> A[i];
		A[i] += A[i-1];
	}
	for(int i = 0; i < N; i ++) {
		for(int j = 0; j < M; j ++) {
			cin >> mat[j][i];
		}
	}
	st.init(N, M);
	st.build(mat);
	long long ans = DivideAndConquerDP(0, N-1, 0, N-1);
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
