/*
	Problem Link :
	* https://codeforces.com/contest/875/problem/E
	
	Idea :
	* We can see the function of maximum length is monotonic function. So if we can do delivery with maxlength "x", then we can do it with "x+1" also.
	
	* We will iterate through the order, we know that at ith delivery one co-ordinate is fixed i.e. Xi, and for other cordinate we will maintain a set.
	
	* Now transition from ith delivery to i+1th delivery will be as, either the guy standing at ith delivery point will go to i+1th point or guy standing
	- at one of the points in the set go to it. 
	
	* In case of ith guy goes, then we will remove all the points which are less than Xi+1-maxLen will be removed and points more than Xi+1+maxLen will 
	- be removed.
	
	* In case previous guy goes, then we will check if abs(Xi-Xi+1) <= maxLen, if this condition satisfies add point Xi to the set.
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
const int maxN = 1e5+1;

int n, s1, s2;
vector<int> delivery(maxN);

bool can(int maxLen) {
	set<int> pts;
	pts.insert(s1);
	int cur = s2;
	for(int i = 0; i < n; i ++) {
		int L = delivery[i]-maxLen, R = delivery[i]+maxLen;
		while(pts.size()) {
			auto it = *pts.begin();
			if(it < L) pts.erase(it);
			else break;
		}
		while(pts.size()) {
			auto it = pts.end();
			it --;
			if(*it > R) pts.erase(it);
			else break;
		}
		if(abs(delivery[i]-cur) <= maxLen) pts.insert(cur);
		cur = delivery[i];
		if(!pts.size()) return false;
	}
	if(pts.size()) return true;
	else return false;
}

void Solve() {
	cin >> n >> s1 >> s2;
	for(int i = 0; i < n; i ++) {
		cin >> delivery[i];
	}
	if(s1 > s2) swap(s1, s2);
	int low = s2-s1, mid, high = 1e9, ans = 1e9;
	while(low <= high) {
		mid = (low + high)/2;
		if(can(mid)) {
			ans = mid;
			high = mid-1;
		}
		else low = mid+1;
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
