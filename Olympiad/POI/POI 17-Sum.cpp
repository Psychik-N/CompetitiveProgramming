/*
	Problem Link :
	* https://szkopul.edu.pl/problemset/problem/Ng815bt4Fko9lj2-l7eVl3Aw/site/
	
	Idea :
	* Precalculate the power of 10 mod m.
	
	* We will do this problem using dp. dp[i][j][k] | i --> ith position, j --> sum to form, k --> required modulo, dp[i][j][k] --> count of numbers
	- satisfying the condition.
	
	* Transition :
	- For dp[i][j][k], you have 10 choices from 0 to 9.
	- Suppose we choose 3, then state it will visit will be dp[i-1][j-3][(k-((pow10[pos] * 3) % m) + m) % m];
	- One thing to note is that we will cap the count till 1e18 as k is not greater than 1e18.
	
	* Now we have to construct the solution for each query. It can be done in O(count of digit) per query. Where count of digit <= 200.
	
	* As we have to create the kth number then for each position(iterate for position from 200 to 1) iterate from 0 to 9 and get total 
	- count which satisfy the condition after assigning ith pos to 0, 1, 2, ... . Now do the cummalative sum of count from 0 to 9 when 
	- cummalative_count >= K, assign that digit to the ith position. and go to the i-1 position(make sure to modify the sum and modulo).
	
	* Note!! if you are assigning digit j to position i, then you have to modify the K = K-cummalative_count_till(j-1).
*/
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
const long long INF = 1e18+1;
const int maxN = 2e2+1;

string ans = "";
int S, M, Q;
long long K;
long long pow10Mod[maxN];
long long dp[maxN][maxN][maxN];

long long rec(int pos, int sum, int modulo) {
	if(pos == 0) {
		if((sum == 0) && (modulo == 0)) return 1;
		else return 0;
	}
	if(sum < 0) return 0;
	if(dp[pos][sum][modulo] != -1) return dp[pos][sum][modulo];
	long long ans = 0;
	for(int i = 0; i < 10; i ++) {
		if(ans > 1e18) break;
		int _modulo = (modulo - ((i * pow10Mod[pos]) % M) + M) % M;
		ans += rec(pos-1, sum-i, _modulo);
	}
	ans = min(INF, ans);
	dp[pos][sum][modulo] = ans;
	return ans;
}

void construct(int pos, int sum, int modulo) {
	if(pos == 0) return;
	long long total = 0;
	long long prev = 0;
	for(int i = 0; i < 10; i ++) {
		int _modulo = (modulo - ((i * pow10Mod[pos]) % M) + M) % M;
		total += rec(pos-1, sum-i, _modulo);
		if(total >= K) {
			K = K-prev;
			ans += char('0'+i);
			int _modulo = (modulo - ((i * pow10Mod[pos]) % M) + M) % M;
			construct(pos-1, sum-i, _modulo);
			return;
		}
		prev = total;
	}
}

void Solve() {
	memset(dp, -1, sizeof(dp));
	cin >> S >> M >> Q;
	pow10Mod[1] = 1;
	for(int i = 2; i < maxN; i ++) {
		pow10Mod[i] = (pow10Mod[i-1] * 10) % M;
	}
	while(Q --) {
		cin >> K;
		ans = "";
		construct(200, S, 0);
		if(ans == "") cout << "NIE\n";
		else {
			int flag = 0;
			for(auto i : ans) {
				if(flag) {
					cout << i;
				}
				else {
					if(i != '0') {
						cout << i;
						flag = 1;
					}
				}
			}
			cout << '\n';
		}
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
