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

int dx[4] = {-1, 1, 0, 0};
int dy[4] = {0, 0, -1, 1};

bool can(int x, int y) {
	return ((x >= 0) && (y >= 0) && (x < 500) && (y < 500));
}

vector<long long> Sieve(int N) {
	vector<long long> primes;
	vector<bool> isPrime(N+1, 0);
	for(int i = 2; i < N+1; i ++) {
		if(isPrime[i] == 0) {
			primes.push_back(i);
			for(int j = i+i; j < N+1; j += i) {
				isPrime[j] = 1;
			}
		}
	}
	return primes; 
}

void Solve() {
	vector<long long> p = Sieve(int(1e5));
	vector<vector<long long>> mat(500, vector<long long>(500, 1));
	vector<pair<int, int>> start;
	int ptr = -1;
	for(int i = 0; i < 500; i += 2) {
		ptr ++;
		int x = 0, y = i;
		while((x < 500) && (y < 500)) {
			mat[x][y] *= p[ptr];
			x ++, y ++;
		}
		if(i == 0) continue;
		ptr ++;
		x = i, y = 0;
		while((x < 500) && (y < 500)) {
			mat[x][y] *= p[ptr];
			x ++, y ++;
		}
	}
	for(int i = 0; i < 500; i += 2) {
		ptr ++;
		int x = i, y = 0;
		while((x >= 0) && (y < 500)) {
			mat[x][y] *= p[ptr];
			x --, y ++;
		}
		ptr ++;
		x = 499, y = i+1;
		while((x >= 0) && (y < 500)) {
			mat[x][y] *= p[ptr];
			x --, y ++;
		}
	}
	for(int i = 0; i < 500; i ++) {
		for(int j = 0; j < 500; j ++) {
			if(mat[i][j] != 1) continue;
			long long LCM = 1;
			for(int k = 0; k < 4; k ++) {
				int x = i+dx[k], y = j+dy[k];
				if(can(x, y)) {
					long long other = mat[x][y];
					long long GCD = __gcd(LCM, other);
					other /= GCD;
					LCM *= other;
				}
			}
			mat[i][j] = LCM+1;
		}
	}
	int N;
	cin >> N;
	for(int i = 0; i < N; i ++) {
		for(int j = 0; j < N; j ++) {
			cout << mat[i][j] << " ";
		}
		cout << endl;
	}
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
