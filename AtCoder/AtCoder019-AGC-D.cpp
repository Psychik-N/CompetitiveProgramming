/*
	Problem Link :
	* https://atcoder.jp/contests/agc019/tasks/agc019_d
	
	Idea :
	* https://atcoder.jp/contests/agc019/editorial
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

const long long mod = 998244353;
const long double PI = acos(-1.0);
const long long INF = 1e18;
const int maxN = 2e5+1;

string A, B;
int ans = 10000, reqSZ[2001][2001], dif[2001];

void computeSZ() {
	vector<int> left(A.size(), -1), right(A.size(), -1);
	int ptr = 0;
	for(int i = B.size()-1; i >= 0; i --) {
		ptr --;
		if(B[i] == '1') break;
	}
	for(int i = 0; i < A.size(); i ++) {
		if(B[i] == '1') ptr = i;
		left[i] = i-ptr;
	}
	ptr = A.size()-1;
	for(int i = 0; i < B.size(); i ++) {
		ptr ++;
		if(B[i] == '1') break;
	}
	for(int i = A.size()-1; i >= 0; i --) {
		if(B[i] == '1') ptr = i;
		right[i] = ptr-i;
	}
	for(int i = 0; i < A.size(); i ++) {
		vector<pair<int, int>> consider;
		for(int j = 0; j < A.size(); j ++) {
			int idx = (i+j) % A.size();
			dif[i] += (A[j] != B[idx]);
			if((B[idx] == '0') && (A[j] == '1')) {
				consider.push_back({left[j], right[j]});
			}
		}
		consider.push_back({-1, -1});
		sort(rALL(consider));
		int rightBorder = 0;
		ptr = 0;
		for(int j = A.size(); j >= 0; j --) {
			while(consider[ptr].first > j) {
				rightBorder = max(rightBorder, consider[ptr].second);
				ptr ++;
			}
			reqSZ[i][j] = rightBorder+j;
		}
	}
}

void computeAns() {
	for(int i = 0; i <= A.size(); i ++) {
		int permut =(i % A.size());
		int req = reqSZ[permut][0];
		for(int j = 0; j < A.size(); j ++) {
			int L = j, R = i+j;
			req = min(reqSZ[permut][L], req);
			if(req <= R) {
				ans = min(ans, dif[permut]+L+R);
			}
		}
	}
	for(int i = 0; i <= A.size(); i ++) {
		int permut = ((A.size()-i) % A.size());
		int req = reqSZ[permut][i];
		for(int j = 0; j < A.size(); j ++) {
			int L = i+j, R = j;
			if(L > A.size()) break;
			req = min(reqSZ[permut][L], req);
			if(req <= L) {
				ans = min(ans, dif[permut]+L+R);
			}
		}
	}
}

void Solve() {
	cin >> A >> B;
	int countA = 0, countB = 0;
	for(int i = 0; i < B.size(); i ++) {
		if(B[i] == '0') countB ++;
		if(A[i] == '0') countA ++;
	}
	if(countB == B.size()) {
		if(countA == A.size()) cout << 0 << endl;
		else cout << -1 << endl;
		return;
	}
	computeSZ();
	computeAns();
	cout << ans << endl;
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
