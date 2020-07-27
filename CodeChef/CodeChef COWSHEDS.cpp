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
	static constexpr int MOD = 1e9+9;
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

struct BIT {
	using T = modint;
	vector<T> arr;
	BIT(int s = 1e6) {
		arr.resize(s+1);
		for(int i = 0; i < s+1; i ++) arr[i] = 0;
	}
	T sum(int pos) {
		T ans = 0;
		while(pos > 0) {
			ans += arr[pos];
			pos = pos - (pos & (-pos));
		}
		return ans;
	}
	void update(int pos, modint val) {
		while((pos > 0) && (pos < arr.size())) {
			arr[pos] += val;
			pos = pos + (pos & (-pos));
		}
	}
	modint rQuery(int l, int r) {
		return sum(r) - sum(l-1);
	}
};

modint inv31 = modint(1)/modint(31);
vector<int> child[maxN], parent(maxN);
BIT hashPref(maxN), hashSuf(maxN), sum(maxN);
vector<modint> power(maxN, 1), invp(maxN, 1);

void Solve() {
	int N, Q;
	cin >> N >> Q;
	for(int i = 1; i < maxN; i ++) {
		child[i].push_back(i);
		parent[i] = i;
		power[i] = power[i-1] * modint(31);
		invp[i] = invp[i-1] * inv31;
	}
	for(int i = 1; i <= N; i ++) {
		hashPref.update(i, modint(i) * power[i]);
		hashSuf.update(i, modint(i) * power[N-i+1]);
		sum.update(i, modint(i) * modint(i));
	}
	int comp = N;
	while(Q --) {
		int L, R;
		cin >> L >> R;
		int limit = L+(R-L)/2;
		if((R-L+1) % 2) limit --;
		for(int i = L; i <= limit; i ++) {
			int low = i, high = limit, mid, ans = -1;
			while(low <= high) {
				mid = (low + high) / 2;
				modint pref = hashPref.rQuery(L, mid) * invp[L];
				modint suf = hashSuf.rQuery(R-mid+L, R) * invp[N-R+1];
				modint psum = sum.rQuery(L, mid);
				modint ssum = sum.rQuery(R-mid+L, R);
				if((int(pref) == int(suf)) && (int(psum) == int(ssum))) {
					low = mid+1;
				}
				else {
					ans = mid;
					high = mid-1;
				}
			}
			if((ans <= limit) && (ans != -1)) {
				int x = parent[ans], y = parent[R-(ans-L)];
				comp --;
				if(child[x].size() < child[y].size()) swap(x, y);
				for(auto j : child[y]) {
					hashPref.update(j, -(modint(y) * power[j]));
					hashPref.update(j, modint(x) * power[j]);
					hashSuf.update(j, -(modint(y) * power[N-j+1]));
					hashSuf.update(j, modint(x) * power[N-j+1]);
					sum.update(j, - (modint(y) * modint(y)));
					sum.update(j, modint(x) * modint(x));
					parent[j] = x;
					child[x].push_back(j);
				}
				child[y].clear();
				i = ans;
			}
			else break;
		}
		cout << comp << endl;
	}
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
