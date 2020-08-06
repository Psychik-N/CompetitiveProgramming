/*
	Problem Link :
	* https://codeforces.com/contest/725/problem/E
	
	Idea : 
	* Algorithm takes at max O(sqrt(c)) steps to fill a bag of size c.
	
	* Need to create a "prev" array, where prev[i] = max denomination coin less than i that is present.
	
	* If we can give a solution with 2 or more coins, then we can give the solution with 1 coin whose denomination will be the sum of
	- denomination of all the coins.
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
const int maxN = 2e5+1;

void Solve() {
	int bag, n;
	vector<int> coin(int(2e5+1), 0);
	vector<int> prev(int(2e5+1), 0);
	vector<int> space(int(2e5+1), 0);
	cin >> bag >> n;
	for(int i = 1; i <= n; i ++) {
		int in;
		cin >> in;
		coin[in] ++;
	}
	int aux = bag;
	for(int i = 2e5; i >= 0; i --) {
		space[i] = aux;
		if(i) {
			int rem = aux % i;
			int req = aux / i;
			if(req > coin[i]) {
				rem += (req-coin[i])*i;
			}
			aux = rem;
		}
	}
	int cur = 0;
	for(int i = 1; i <= 2e5; i ++) {
		prev[i] = cur;
		if(coin[i]) cur = i;
	}
	int block = sqrt(bag)+2;
	vector<vector<bool>> possible(block+1, vector<bool>(bag+1, false));
	possible[0][0] = 1;
	for(int i = 1; i <= block; i ++) {
		possible[i][0] = 1;
		for(int j = 1; j <= bag; j ++) {
			int rem = j % i;
			int req = j / i;
			if(req > coin[i]) {
				rem += (req-coin[i])*i;
			}
			possible[i][j] = possible[i-1][rem];
		}
	}
	if(space[0]) {
		cout << 0 << endl;
		return;
	}
	for(int i = 1; i < bag; i ++) {
		if(space[i-1] < i) continue;
		int tofill = space[i-1]-i;
		int ptr = i-1;
		while(ptr > block) {
			if(coin[ptr]) {
				int rem = tofill % ptr;
				int req = tofill / ptr;
				if(req > coin[ptr]) {
					rem += (req-coin[ptr])*ptr;
				}
				tofill = rem;
				ptr = min(prev[ptr], tofill);
			}
			else ptr = prev[ptr];
		}
		if(!possible[ptr][tofill]) {
			cout << i << endl;
			return;
		}
	}
	cout << "Greed is good\n";
	return;
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
