/*
	Problem Link :
	* https://codeforces.com/contest/600/problem/E
	
	Idea :
	* DSU on Trees, also known as Sack.
	
	* You can read about DSU on Trees here = https://codeforces.com/blog/entry/44351
	
	* Solution = https://codeforces.com/blog/entry/21827
*/
#pragma GCC optimize("O3")
#pragma GCC target("sse4")

#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define ld long double
#define v vector
#define ALL(x) x.begin(),x.end()
#define rALL(x) x.rbegin(),x.rend()
#define endl '\n'

const ll mod = 1e9 + 7;
const ld PI = acos(-1.0);

struct SACK {
	using T = long long;
	T N, root = 1;
	vector<vector<T>> graph;
	vector<T> subtree;
	vector<bool> isBig;
	// Some type of info. specific to the question.
		vector<T> color, freq, ans, cnt;
		set<T> maintain;
	// finish.
	SACK(T n = 1e6) {
		N = n;
		graph.resize(N+1);
		subtree.resize(N+1);
		color.resize(N+1);
		freq.resize(N+1, 0);
		ans.resize(N+1, 0);
		cnt.resize(N+1, 0);
		isBig.resize(N+1, false);
	}
	// Function below is specific to the question.
	void takeInput() {
		root = 1;
		for(int i = 1; i < N+1; i ++) {
			cin >> color[i];
		}
		for(int i = 0; i < N-1; i ++) {
			T a, b;
			cin >> a >> b;
			graph[a].push_back(b);
			graph[b].push_back(a);
		}
	}
	void computeSubtree(T node, T parent = -1) {
		subtree[node] = 1;
		for(auto i : graph[node]) {
			if(i == parent) continue;
			computeSubtree(i, node);
			subtree[node] += subtree[i];
		}
	}
	// Function below is specific to the question.
	void add(T node, T parent, T val) {
		if(freq[cnt[color[node]]] - color[node] == 0) maintain.erase(cnt[color[node]]);
		freq[cnt[color[node]]] -= color[node];
		cnt[color[node]] += val;
		maintain.insert(cnt[color[node]]);
		freq[cnt[color[node]]] += color[node];
		for(auto i : graph[node]) {
			if((i != parent) && !isBig[i])
				add(i, node, val);
		}
	}
	void runSack(T node, T parent, bool keep) {
		T bigChild = -1, maxSize = -1;
		for(auto i : graph[node]) {
			if((subtree[i] > maxSize) && (i != parent)) {
				maxSize = subtree[i];
				bigChild = i; 
			}
		}
		for(auto i : graph[node]) {
			if((i != parent) && (i != bigChild)) {
				runSack(i, node, 0);
			}
		}
		if(bigChild != -1) {
			runSack(bigChild, node, 1);
			isBig[bigChild] = 1;
		}
		add(node, parent, 1);
		ans[node] = freq[*(--maintain.end())];
		if(bigChild != -1) {
			isBig[bigChild] = false;
		}
		if(keep == 0) {
			add(node, parent, -1);
		}
	}
};

int main() {
	ios_base::sync_with_stdio(0);
	cin.tie(0);
	cout.tie(0);

	ll N;
	cin >> N;
	SACK dt(N);
	dt.takeInput();
	dt.computeSubtree(1, -1);
	dt.runSack(1, 1, 0);
	for(ll i = 1; i <= N; i ++) {
		cout << dt.ans[i] << " "; 
	}
	cout << endl;

	return 0;
}
