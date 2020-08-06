/*
	Problem Link :
	* https://www.spoj.com/problems/EASYFACT/
	
	Idea :
	* let N! = (a+1) + (a+2) ... (a+l). Applying AP formula 2*N! = (l*(2*a+l-1)), where l and (2*a+l-1) have different parity. So number of these pair
	- combination satisfying these properties will be equal to number of odd divisors of N!.
	
	* You have to optimize it, to fit in time limit.
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

const long long mod = 998244353;
const long double PI = acos(-1.0);
const long long INF = 1e18;
const int maxN = 1e8+1;

template<typename T> T FastPower(T num, T raise, T modulo) {
	T result = 1;
	while(raise > 0) {
		if(raise & 1) result = (result * num) % modulo;
		num = (num * num) % modulo;
		raise = (raise >> 1);
	}
	return result;
}

vector<int> prime;

void Solve() {
	long long N, M;
	cin >> N >> M;
	long long ways = 1;
	for(int i = 1; i < prime.size(); i ++) {
		if(prime[i] > N) break;
		long long x = prime[i];
		long long count = 0;
		while(x <= N) {
			count += N/x;
			x = (x*((long long)(prime[i])));
		} 
		if(prime[i] < 1e4) {
			count = (count+1) % M;
			ways = (ways * count) % M;
		}
		else {
			int low = i, mid, ans = i, high = prime.size()-1;
			while(low <= high) {
				mid = (low + high) / 2;
				long long xx = prime[mid];
				long long cc = N/xx;
				if(cc == count) {
					ans = mid;
					low = mid+1;
				}
				else high = mid-1;
			}
			count = (count+1) % M;
			ways = (ways * FastPower<long long>(count, ans-i+1, M)) % M;
			i = ans;
		}
	}
	ways --;
	if(ways < 0) ways += M;
	cout << ways << endl;
}

int main() {
	ios_base::sync_with_stdio(0);
	cin.tie(0);
	cout.tie(0);

	{
		prime.push_back(2);
		vector<bool> sieve(maxN, true);
		for(int i = 3; i <= maxN; i += 2) {
			if(sieve[i]) {
				prime.push_back(i);
				for(int j = 3*i; j <= maxN; j += 2*i) {
					sieve[j] = false;
				}
			}
		}
	}

	int tt = 1;
	cin >> tt;
	for(int i = 1; i <= tt; i ++) {
		Solve();
	}
	
	return 0;
}
