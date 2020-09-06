/*
	Problem Link : 
	* https://atcoder.jp/contests/agc043/tasks/agc043_d
	
	Idea : 
	* https://atcoder.jp/contests/agc043/editorial
*/

#include <bits/stdc++.h>
using namespace std;

long long dp[6005][9005];

int main() {
	ios_base::sync_with_stdio(0);
	cin.tie(0);
	cout.tie(0);
	
	int offset = 3000;

	long long N, M;
	cin >> N >> M;
	
	dp[0][offset] = 1;
	for(int i = 0; i < 3*N; i ++) {
		for(int j = -N; j <= 3*N; j ++) {
			dp[i+1][j+1+offset] = (dp[i+1][j+1+offset] + dp[i][j+offset]) % M;
			dp[i+2][j-1+offset] = (dp[i+2][j-1+offset] + dp[i][j+offset] * (i+1) % M) % M;
			dp[i+3][j+offset] = (dp[i+3][j+offset] + dp[i][j+offset] * (i+1) % M * (i+2) % M) % M;
		}
	}
	long long ans = 0;
	for(int i = 0; i <= N; i ++) ans = (ans + dp[3*N][3*i+offset]) % M;
	cout << ans << '\n';
	return 0;
}
