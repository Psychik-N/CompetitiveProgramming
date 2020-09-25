/*
	Problem Link :
	* https://codeforces.com/contest/1262/problem/F2
	
	Idea :
	* https://codeforces.com/blog/entry/71740
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

const long long mod = 1e9 + 7;
const long double PI = acos(-1.0);

struct modint {
	static constexpr int MOD = 998244353;
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

modint FastPower(modint num, int raise) {
	modint result = 1;
	while(raise > 0) {
		if(raise & 1) result = (result * num);
		num = (num * num);
		raise = (raise >> 1);
	}
	return result;
}

modint fac[int(2e5+1)], ifac[int(2e5+1)];

modint nCr(int n, int r) {
	if(r > n) return 0;
	return (fac[n] * ifac[n-r] * ifac[r]);
}

void Solve() {
	fac[0] = ifac[0] = 1;
	for(int i = 1; i <= int(2e5); i ++) {
		fac[i] = fac[i-1] * modint(i);
		ifac[i] = (ifac[i-1] / modint(i));
	}
	int n, k;
	cin >> n >> k;
	int same = 0, dif = 0;
	vector<int> correct(n);
	for(int i = 0; i < n; i ++) {
		cin >> correct[i];
	}
	for(int i = 0; i < n; i ++) {
		if(correct[i] == correct[(i+1)%n]) same ++;
		else dif ++;
	}
	modint ways = 0;
	modint mul = 1;
	for(int i = 0; i < dif; i ++) {
		int rem = dif-i;
		modint res = FastPower(2, rem-1);
		if((rem % 2) == 0) res -= (nCr(rem, rem/2) / modint(2));
		res *= mul;
		res *= nCr(dif, i);
		ways += res;
		mul *= modint(k-2);
	}
	for(int i = 0; i < same; i ++) ways *= modint(k);
	cout << ways << "\n";
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
