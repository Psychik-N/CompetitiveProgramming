/*
	Problem Link :
	* Problem H in the attachment link = https://codeforces.com/gym/100197/attachments
	
	Idea :
	* It requires knowledge of burnside lemma, polya's enumeration theorem and group theory.
	
	* You can learn about lemma's here = https://cp-algorithms.com/combinatorics/burnside.html
	
	* For group theory you can search it on brilliant.org
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

const ll mod = 1e9 + 7;
const ld PI = acos(-1.0);
const ll maxN = 1e5+1;

struct Int {
	static const int base = 1000000000;
	static const int BaseDigit = 9;
	vector<int> a;
	int sign;   
	int size() {
		if(a.empty()) return 0;
		int ans = (a.size()-1)*BaseDigit;
		int ca = a.back();
		while(ca)
			ans++, ca /= 10;
		return ans;
	}
	Int operator ^(const Int &vv){
		Int ans = 1, aa = *this, b = vv;
		while(!b.isZero()){
			if(b % 2)
			ans *= aa;
			aa *= aa, b /= 2;
		}
		return ans;
	}
	string to_string(){
		stringstream ss;
		ss << *this;
		string s;
		ss >> s;
		return s;
	} 
	int sumof(){
		string s = to_string();
		int ans = 0;
		for(auto c : s)  ans += c - '0';
		return ans;
	}
	Int() :
	sign(1) {
	}
	Int(long long vv) {
		*this = vv;
	}
	Int(const string &s) {
		read(s);
	}
	void operator = (const Int &vv) {
		sign = vv.sign;
		a = vv.a;
	}
	void operator = (long long vv) {
		sign = 1;
		a.clear();
		if (vv < 0)
			sign = -1, vv = -vv;
		for (; vv > 0; vv = vv / base)
			a.push_back(vv % base);
	}
	Int operator + (const Int &vv) const {
		if (sign == vv.sign) {
			Int res = vv;
			for (int i = 0, carry = 0; i < (int) max(a.size(), vv.a.size()) || carry; ++i) {
				if (i == (int) res.a.size())
					res.a.push_back(0);
				res.a[i] += carry + (i < (int) a.size() ? a[i] : 0);
				carry = res.a[i] >= base;
				if (carry)
					res.a[i] -= base;
			}
			return res;
		}
		return *this - (-vv);
	}
	Int operator - (const Int &vv) const {
		if (sign == vv.sign) {
			if (abs() >= vv.abs()) {
				Int res = *this;
				for (int i = 0, carry = 0; i < (int) vv.a.size() || carry; ++i) {
					res.a[i] -= carry + (i < (int) vv.a.size() ? vv.a[i] : 0);
					carry = res.a[i] < 0;
					if (carry)
					res.a[i] += base;
				}
				res.trim();
				return res;
			}
			return -(vv - *this);
		}
		return *this + (-vv);
	} 
	void operator *= (int vv) {
		if (vv < 0)
			sign = -sign, vv = -vv;
		for (int i = 0, carry = 0; i < (int) a.size() || carry; ++i) {
			if (i == (int) a.size())
				a.push_back(0);
			long long cur = a[i] * (long long) vv + carry;
			carry = (int) (cur / base);
			a[i] = (int) (cur % base);
		}
		trim();
	}
 
	Int operator * (int vv) const {
		Int res = *this;
		res *= vv;
		return res;
	}
	void operator *= (long long vv) {
		if (vv < 0)
			sign = -sign, vv = -vv;
		for (int i = 0, carry = 0; i < (int) a.size() || carry; ++i) {
			if (i == (int) a.size())
				a.push_back(0);
			long long cur = a[i] * (long long) vv + carry;
			carry = (int) (cur / base);
			a[i] = (int) (cur % base);
		}
		trim();
	} 
	Int operator * (long long vv) const {
		Int res = *this;
		res *= vv;
		return res;
	}
	friend pair<Int, Int> divmod(const Int &a1, const Int &b1) {
		int norm = base / (b1.a.back() + 1);
		Int a = a1.abs() * norm;
		Int b = b1.abs() * norm;
		Int q, r;
		q.a.resize(a.a.size());
		for (int i = a.a.size() - 1; i >= 0; i--) {
			r *= base;
			r += a.a[i];
			int s1 = r.a.size() <= b.a.size() ? 0 : r.a[b.a.size()];
			int s2 = r.a.size() <= b.a.size() - 1 ? 0 : r.a[b.a.size() - 1];
			int d = ((long long) base * s1 + s2) / b.a.back();
			r -= b * d;
			while (r < 0)
				r += b, --d;
			q.a[i] = d;
		}
		q.sign = a1.sign * b1.sign;
		r.sign = a1.sign;
		q.trim();
		r.trim();
		return make_pair(q, r / norm);
	}
	Int operator / (const Int &vv) const {
		return divmod(*this, vv).first;
	}
	Int operator % (const Int &vv) const {
		return divmod(*this, vv).second;
	}
	void operator /= (int vv) {
		if (vv < 0)
			sign = -sign, vv = -vv;
		for (int i = (int) a.size() - 1, rem = 0; i >= 0; --i) {
			long long cur = a[i] + rem * (long long) base;
			a[i] = (int) (cur / vv);
			rem = (int) (cur % vv);
		}
		trim();
	}
	Int operator/(int vv) const {
		Int res = *this;
		res /= vv;
		return res;
	}
 
	int operator%(int vv) const {
		if (vv < 0)
			vv = -vv;
		int m = 0;
		for (int i = a.size() - 1; i >= 0; --i)
			m = (a[i] + m * (long long) base) % vv;
		return m * sign;
	}
	void operator += (const Int &vv) {
		*this = *this + vv;
	}
	void operator -= (const Int &vv) {
		*this = *this - vv;
	}
	void operator *= (const Int &vv) {
		*this = *this * vv;
	}
	void operator /= (const Int &vv) {
		*this = *this / vv;
	}
	Int operator ++ () {
		*this += 1;
		return *this;
	}
	Int operator ++ (int) {
		Int temp = *this;
		*this += 1;
		return temp;
	}
	Int operator --() {
		*this -= 1;
		return *this;
	}
	Int operator --(int) {
		Int temp = *this;
		*this -= 1;
		return temp;
	}
	bool operator < (const Int &vv) const {
		if (sign != vv.sign)
			return sign < vv.sign;
		if (a.size() != vv.a.size())
		   return a.size() * sign < vv.a.size() * vv.sign;
		for (int i = a.size() - 1; i >= 0; i--)
			if (a[i] != vv.a[i])
				return a[i] * sign < vv.a[i] * sign;
		return false;
	}
	bool operator > (const Int &vv) const {
		return vv < *this;
	}
	bool operator <= (const Int &vv) const {
		return !(vv < *this);
	}
	bool operator >= (const Int &vv) const {
		return !(*this < vv);
	}
	bool operator == (const Int &vv) const {
		return !(*this < vv) && !(vv < *this);
	}
	bool operator != (const Int &vv) const {
		return *this < vv || vv < *this;
	}
	void trim() {
		while (!a.empty() && !a.back())
			a.pop_back();
		if (a.empty())
			sign = 1;
	}
	bool isZero() const {
		return a.empty() || (a.size() == 1 && !a[0]);
	}
	Int operator - () const {
		Int res = *this;
		res.sign = -sign;
		return res;
	}
	Int abs() const {
		Int res = *this;
		res.sign *= res.sign;
		return res;
	}
	long long longValue() const {
		long long res = 0;
		for (int i = a.size() - 1; i >= 0; i--)
			res = res * base + a[i];
		return res * sign;
	}
	friend Int gcd(const Int &a, const Int &b) {
		return b.isZero() ? a : gcd(b, a % b);
	}
	friend Int lcm(const Int &a, const Int &b) {
		return a / gcd(a, b) * b;
	}
	void read(const string &s) {
		sign = 1;
		a.clear();
		int pos = 0;
		while (pos < (int) s.size() && (s[pos] == '-' || s[pos] == '+')) {
			if (s[pos] == '-')
				sign = -sign;
			++pos;
		}
		for (int i = s.size() - 1; i >= pos; i -= BaseDigit) {
			int x = 0;
			for (int j = max(pos, i - BaseDigit + 1); j <= i; j++)
				x = x * 10 + s[j] - '0';
			a.push_back(x);
		}
		trim();
	}
	friend istream& operator >> (istream &stream, Int &vv) {
		string s;
		stream >> s;
		vv.read(s);
		return stream;
	}
	friend ostream& operator << (ostream &stream, const Int &vv) {
		if (vv.sign == -1)
		   stream << '-';
		stream << (vv.a.empty() ? 0 : vv.a.back());
		for (int i = (int) vv.a.size() - 2; i >= 0; --i)
			stream << setw(BaseDigit) << setfill('0') << vv.a[i];
		return stream;
	}
	static vector<int> convert_base(const vector<int> &a, int old_digits, int new_digits) {
		vector<long long> p(max(old_digits, new_digits) + 1);
		p[0] = 1;
		for (int i = 1; i < (int) p.size(); i++)
			p[i] = p[i - 1] * 10;
		vector<int> res;
		long long cur = 0;
		int cur_digits = 0;
		for (int i = 0; i < (int) a.size(); i++) {
			cur += a[i] * p[cur_digits];
			cur_digits += old_digits;
			while (cur_digits >= new_digits) {
				res.push_back(int(cur % p[new_digits]));
				cur /= p[new_digits];
				cur_digits -= new_digits;
			}
		}
		res.push_back((int) cur);
		while (!res.empty() && !res.back())
			res.pop_back();
		return res;
	}
	typedef vector<long long> vll;
	static vll karatsubaMultiply(const vll &a, const vll &b) {
		int n = a.size();
		vll res(n + n);
		if (n <= 32) {
			for (int i = 0; i < n; i++)
				for (int j = 0; j < n; j++)
					res[i + j] += a[i] * b[j];
			return res;
		}
		int k = n >> 1;
		vll a1(a.begin(), a.begin() + k);
		vll a2(a.begin() + k, a.end());
		vll b1(b.begin(), b.begin() + k);
		vll b2(b.begin() + k, b.end());
		vll a1b1 = karatsubaMultiply(a1, b1);
		vll a2b2 = karatsubaMultiply(a2, b2);
		for (int i = 0; i < k; i++)
			a2[i] += a1[i];
		for (int i = 0; i < k; i++)
			b2[i] += b1[i];
		vll r = karatsubaMultiply(a2, b2);
		for (int i = 0; i < (int) a1b1.size(); i++)
			r[i] -= a1b1[i];
		for (int i = 0; i < (int) a2b2.size(); i++)
			r[i] -= a2b2[i];
		for (int i = 0; i < (int) r.size(); i++)
			res[i + k] += r[i];
		for (int i = 0; i < (int) a1b1.size(); i++)
			res[i] += a1b1[i];
		for (int i = 0; i < (int) a2b2.size(); i++)
			res[i + n] += a2b2[i];
		return res;
	}
	Int operator * (const Int &vv) const {
		vector<int> a6 = convert_base(this->a, BaseDigit, 6);
		vector<int> b6 = convert_base(vv.a, BaseDigit, 6);
	   
		vll a(a6.begin(), a6.end());
		vll b(b6.begin(), b6.end());
	   
		while (a.size() < b.size())
			a.push_back(0);
		while (b.size() < a.size())
			b.push_back(0);
		while (a.size() & (a.size() - 1))
			a.push_back(0), b.push_back(0);
		vll c = karatsubaMultiply(a, b);
		Int res;
		res.sign = sign * vv.sign;
		for (int i = 0, carry = 0; i < (int) c.size(); i++) {
			long long cur = c[i] + carry;
			res.a.push_back((int) (cur % 1000000));
			carry = (int) (cur / 1000000);
		}
		res.a = convert_base(res.a, 6, BaseDigit);
		res.trim();
		return res;
	}
	friend Int max(const Int &a,const Int &b){
		if(a<b){
			return a;
		}
		return b;
	}
	friend Int max(const Int &a,const int32_t &B){
		Int b = B;
		return max(a,b);
	}
	friend Int max(const Int &a,const int64_t &B){
		Int b = B;
		return max(a,b);
	}
	friend Int min(const Int &a,const Int &b){
		if(a>b){
			return b;
		}
		return a;
	}
	friend Int min(const Int &a,const int32_t &B){
		Int b = B;
		return min(a,b);
	}
	friend Int min(const Int &a,const int64_t &B){
		Int b = B;
		return min(a,b);
	}
	friend Int pow(const Int &a,const Int &b){
		Int c = 1;
		for(Int i=1; i<=b; i+=1){
			c *= a;
		}
		return c;
	}
	friend Int pow(const Int &a,const int32_t &B){
		Int b = B;
		return pow(a,b);
	}
	friend Int pow(const Int &a,const int64_t &B){
		Int b = B;
		return pow(a,b);
	}
	friend Int sqrt(Int a) {
		Int x0 = a, x1 = (a+1)/2;
		while (x1 < x0) {
			x0 = x1;
			x1 = (x1+a/x1)/2;
		}
		return x0;
	}
};

struct DSU {
	using T = int;
	vector<T> parent;
	vector<T> size;
	T numComponents = 0;
	DSU(T n = 0) {
		if(n >= 0)
			init(n);
	}
	void init(T n = 0) {
		parent.resize(n);
		size.assign(n, 1);
		numComponents = n;
		for (T i = 0; i < n; i++)
			parent[i] = i;
	}
	T getParent(T x) {
		return (x == parent[x]) ? x : parent[x] = getParent(parent[x]);
	}
	bool merge(T x, T y) {
		x = getParent(x);
		y = getParent(y);
		if (x == y) {
			return false;
		} 
		if (size[x] < size[y]) {
			swap(x, y);
		}
		parent[y] = x;
		size[x] += size[y];
		numComponents--;
		return true;
	}
};

int N, M;
Int powTwo[500];
DSU dif;
vector<vector<int>> cur;
vector<vector<int>> pts;

int getIdx(int x, int y) {
	return (x*M + y);
}

void ShiftHorizontal(int val) {
	for(int i = 0; i < N; i ++) {
		for(int j = 0; j < M; j ++) {
			int I = cur[i][j]/M;
			int J = cur[i][j]%M;
			int x = I;
			int y = (J + val) % M;
			pts[i][j] = getIdx(x, y);
			
		}
	}
	cur = pts;
}

void ShiftVertical(int val) {
	for(int i = 0; i < N; i ++) {
		for(int j = 0; j < M; j ++) {
			int I = cur[i][j]/M;
			int J = cur[i][j]%M;
			int x = (I + val) % N;
			int y = J;
			pts[i][j] = getIdx(x, y);
		}
	}
	cur = pts;
}

void rotate90() {
	if(N != M) return;
	for(int i = 0; i < N; i ++) {
		for(int j = 0; j < M; j ++) {
			int I = cur[i][j]/M;
			int J = cur[i][j]%M;
			int x = J;
			int y = N-1-I;
			pts[i][j] = getIdx(x, y);
		}
	}
	cur = pts;
}

void rotate180() {
	for(int i = 0; i < N; i ++) {
		for(int j = 0; j < M; j ++) {
			int I = cur[i][j]/M;
			int J = cur[i][j]%M;
			int x = N-1-I;
			int y = M-1-J;
			pts[i][j] = getIdx(x, y);
		}
	}
	cur = pts;
}

void Solve() {
	cin >> N >> M;
	powTwo[0] = 1;
	for(int i = 1; i < 500; i ++) {
		powTwo[i] = (powTwo[i-1] + powTwo[i-1]);
	}
	Int ans = 0;
	if(N > M) swap(N, M);
	cur.resize(N, vector<int>(M));
	pts.resize(N, vector<int>(M));
	for(int i = 0; i < N; i ++) {
		for(int j = 0; j < M; j ++) {
			for(int k = 0; k < 2; k ++) {
				for(int x = 0; x < N; x ++) {
					for(int y = 0; y < M; y ++) {
						cur[x][y] = getIdx(x, y);
					}
				}
				dif.init(N*M);
				ShiftVertical(i);
				ShiftHorizontal(j);
				if(k) rotate180();
				for(int x = 0; x < N; x ++) {
					for(int y = 0; y < M; y ++) {
						dif.merge(cur[x][y], getIdx(x, y));
					}
				}
				ans += powTwo[dif.numComponents];
			}
		}
	}
	if(N == M) {
		for(int i = 0; i < N; i ++) {
			for(int j = 0; j < M; j ++) {
				for(int k = 0; k < 2; k ++) {
					for(int x = 0; x < N; x ++) {
						for(int y = 0; y < M; y ++) {
							cur[x][y] = getIdx(x, y);
						}
					}
					dif.init(N*M);
					ShiftVertical(i);
					ShiftHorizontal(j);
					rotate90();
					if(k) rotate180();
					for(int x = 0; x < N; x ++) {
						for(int y = 0; y < M; y ++) {
							dif.merge(cur[x][y], getIdx(x, y));
						}
					}
					ans += powTwo[dif.numComponents];
				}
			}
		}
	}
	if(N == M) ans /= (4*N*M);
	else ans /= (2*N*M);
	cout << ans << endl;
}

int main() {
	ios_base::sync_with_stdio(0);
	cin.tie(0);
	cout.tie(0);

	freopen("tickets.in", "r", stdin);
	freopen("tickets.out", "w", stdout);
	
	int tt = 1;
	while(tt --) {
		Solve();
	}
	
	return 0;
}
