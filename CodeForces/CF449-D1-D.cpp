/*
	Problem Link :
	* https://codeforces.com/contest/449/problem/D
	
	Idea : 
	* https://codeforces.com/blog/entry/13112
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
	int N;
	cin >> N;
	vector<vector<int>> f(21, vector<int>(int(1e6+1), 0));
	for(int i = 0; i < N; i ++) {
		int in;
		cin >> in;
		f[0][in] ++;
	}
	for(int i = 1e6; i >= 0; i --) {
		for(int j = 1; j <= 20; j ++) {
			if(i & (1 << (j-1))) {
				f[j][i] = f[j-1][i];
			}
			else {
				f[j][i] = f[j-1][i];
				if(i+(1 << (j-1)) <= 1e6) {
					f[j][i] += f[j-1][i+(1 << (j-1))];
				}
			}
		}
	}
	vector<modint> pow2(int(1e6+1));
	pow2[0] = 1;
	for(int i = 1; i <= 1e6; i ++) {
		pow2[i] = (pow2[i-1] * modint(2));
	}
	modint ans = 0;
	for(int i = 0; i <= 1e6; i ++) {
		int bitcount = __builtin_popcount(i);
		if(bitcount % 2) ans -= (pow2[f[20][i]]-1);
		else ans += (pow2[f[20][i]]-1);
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
