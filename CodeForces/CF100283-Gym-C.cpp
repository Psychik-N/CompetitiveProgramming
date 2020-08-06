/*
	Problem Link :
	* https://codeforces.com/gym/100283/problem/C
	
	Idea : 
	* This problem uses DP. dp[i][x][y] = total value of thief standing at point x, y and with time i.
	
	* As you can note that each dp is sum of dp states which are diamond in shape so we can do is to rotate it 45 degree to make the problem easier.
	
	* Rotation can be done by (x, y) ---> (x+y, x-y).
	
	* Apply the concept of 2D prefix to compute dp[i][x][y] in O(1).
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

struct modint {
	static constexpr int MOD = 1e9+7;
	using lli = long long;
	int vv;
	static int minv(int a, int m) {
		a %= m;
		assert(a);
		return a == 1 ? 1 : int(m - lli(minv(m, a)) * lli(m) / a);
	}
	modint() : vv(0) {}
	modint(lli v_) : vv(int(v_ % MOD)) { if (vv < 0) vv += MOD; }
	explicit operator int() const { return vv; }
	friend std::ostream& operator << (std::ostream& out, const modint& n) { return out << int(n); }
	friend std::istream& operator >> (std::istream& in, modint& n) { lli v_; in >> v_; n = modint(v_); return in; }
	friend bool operator == (const modint& a, const modint& b) { return a.vv == b.vv; }
	friend bool operator != (const modint& a, const modint& b) { return a.vv != b.vv; }
	modint inv() const {
		modint res;
		res.vv = minv(vv, MOD);
		return res;
	}
	friend modint inv(const modint& m) { return m.inv(); }
	modint neg() const {
		modint res;
		res.vv = vv ? MOD-vv : 0;
		return res;
	}
	friend modint neg(const modint& m) { return m.neg(); }
	modint operator- () const {
		return neg();
	}
	modint operator+ () const {
		return modint(*this);
	}
	modint& operator ++ () {
		vv ++;
		if (vv == MOD) vv = 0;
		return *this;
	}
	modint& operator -- () {
		if (vv == 0) vv = MOD;
		vv --;
		return *this;
	}
	modint& operator += (const modint& o) {
		vv += o.vv;
		if (vv >= MOD) vv -= MOD;
		return *this;
	}
	modint& operator -= (const modint& o) {
		vv -= o.vv;
		if (vv < 0) vv += MOD;
		return *this;
	}
	modint& operator *= (const modint& o) {
		vv = int(lli(vv) * lli(o.vv) % MOD);
		return *this;
	}
	modint& operator /= (const modint& o) {
		return *this *= o.inv();
	}
	friend modint operator ++ (modint& a, int) { modint r = a; ++a; return r; }
	friend modint operator -- (modint& a, int) { modint r = a; --a; return r; }
	friend modint operator + (const modint& a, const modint& b) { return modint(a) += b; }
	friend modint operator - (const modint& a, const modint& b) { return modint(a) -= b; }
	friend modint operator * (const modint& a, const modint& b) { return modint(a) *= b; }
	friend modint operator / (const modint& a, const modint& b) { return modint(a) /= b; }
};

void Solve() {
	int N, M, S, px, py;
	cin >> N >> M >> S >> px >> py;
	vector<vector<int>> grid(N+1, vector<int>(M+1));
	for(int i = 1; i <= N; i ++) {
		for(int j = 1; j <= M; j ++) {
			cin >> grid[i][j];
		}
	}
	modint dp[S+1][N+M+1][N+M+1], prefix2D[S+1][N+M+1][N+M+1];
	for(int i = 1; i <= N; i ++) {
		for(int j = 1; j <= M; j ++) {
			dp[1][i+j][i-j+M] = grid[i][j];
		}
	}
	for(int i = 1; i < S; i ++) {
		for(int x = 1; x <= N+M; x ++) {
			for(int y = 1; y <= N+M; y ++) {
				prefix2D[i][x][y] = dp[i][x][y]+prefix2D[i][x][y-1];
			}
		}
		for(int y = 1; y <= N+M; y ++) {
			for(int x = 1; x <= N+M; x ++) {
				prefix2D[i][x][y] += prefix2D[i][x-1][y];
			}
		}
		for(int xx = 1; xx <= N; xx ++) {
			for(int yy = 1; yy <= M; yy ++) {
				int xMin = 1e9, xMax = -1e9, yMin = 1e9, yMax = -1e9;
				xMin = max(1, xx-i+yy);
				xMax = min(N+M, xx+i+yy);
				yMin = max(1, xx-i-yy+M);
				yMax = min(N+M, xx+i-yy+M);
				dp[i+1][xx+yy][xx-yy+M] = prefix2D[i][xMax][yMax]-prefix2D[i][xMax][yMin-1]-prefix2D[i][xMin-1][yMax]+prefix2D[i][xMin-1][yMin-1];
			}
		}
	}
	cout << dp[S][px+py][px-py+M] << endl;
}

int main() {
	ios_base::sync_with_stdio(0);
	cin.tie(0);
	cout.tie(0);

	freopen("treasures.in", "r", stdin);

	int tt = 1;
	cin >> tt;
	for(int i = 1; i <= tt; i ++) {
		cout << "Case " << i << ": ";
		Solve();
	}
	
	return 0;
}
