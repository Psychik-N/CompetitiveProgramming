/*
	Problem Link :
	* https://icpcarchive.ecs.baylor.edu/index.php?option=com_onlinejudge&Itemid=8&page=show_problem&problem=5248
	
	Idea :
	* We will do it with dp on trees.
	
	* Sibling - nodes which are at same depth and has same parent(not ancestor).
	
	* nextSibling[x] == y, if it fullfil the condition of Sibling and is at immediate right of x, if there is no node at immediate right of x then 
	- nextSibling[x] = -1.
	
	* dp[i][j] = minimum cost to convert subtree of node i from tree 1 to subtree of node j from tree 2 and all there right sibling.
	
	* Let's for now consider node A from tree 1 and node B from tree 2. And forget About it's sibling for now and converting cost will be refered as "self" :
	- When both have child
	- self = (label[A] != label[B]) + dp[first_child[A]][first_child[B]];
	- When A is leaf
	- self = (label[A] != label[B]) + subtree[B] - 1;
	- When B is leaf
	- self = (label[A] != label[B]) + subtree[A] - 1;
	
	* Time required to compute self = O(1).
	
	* Also you have to compute dp states in proper order. Order of evaluation is as follows :
	- For a given node compute dp states for its child first, and from child the right most child first. 
	
	* Now using self how will I get dp[A][B] in O(1), there are some cases which I handled in getCost function:
	- Insert of subtree :
	- dp[A][B] = min(dp[A][B], subtree[B] + dp[A][nextSibling[B]]), where dp[A][nextSibling[B]] will already be computed with proper order of evaluation.
	- Replace of subtree :
	- dp[A][B] = min(dp[A][B], self + dp[nextSibling[A]][nextSibling[B]]).
	- Delete of subtree :
	- dp[A][B] = min(dp[A][B], subtree[A] + dp[nextSibling[A]][B]).
	
	* In this way each node self = (label[A] != label[B]) + dp[first_child[A]][first_child[B]], will give the lowest cost to convert subtree of A to
	- subtree of B.
	
	* Note you have to handle cases where nextSibling[x] == -1.
	
	* Can be made much faster using pruning.
	
	* Time complexity O(N*N).
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

vector<vector<int>> tree[2];
vector<int> nextSibling[2], subtree[2], label[2], suf[2]; 
vector<vector<int>> dp;

void computeInfo(int node, int ID) {
	subtree[ID][node] = 1;
	for(auto i : tree[ID][node]) {
		computeInfo(i, ID);
		subtree[ID][node] += subtree[ID][i];
	}
	for(int i = 0; i < int(tree[ID][node].size())-1; i ++) {
		int Fi = tree[ID][node][i], Se = tree[ID][node][i+1];
		nextSibling[ID][Fi] = Se;
	}
	if(tree[ID][node].size() == 0) return;
	int last = tree[ID][node].size()-1;
	suf[ID][tree[ID][node][last]] = subtree[ID][tree[ID][node][last]];
	for(int i = last-1; i >= 0; i --) {
		suf[ID][tree[ID][node][i]] += suf[ID][tree[ID][node][i+1]] + subtree[ID][tree[ID][node][i]];
	}
}

inline int getCost(int A, int B) {
	int sibA = nextSibling[0][A];
	int sibB = nextSibling[1][B];
	int self = (label[0][A] != label[1][B]);
	bool isLeafA = (tree[0][A].size() == 0);
	bool isLeafB = (tree[1][B].size() == 0);
	int cost = 1e6;
	if(isLeafA) {
		self += subtree[1][B]-1;
	} 
	else if(isLeafB) {
		self += subtree[0][A]-1;
	}
	else {
		self += dp[tree[0][A][0]][tree[1][B][0]];
	}
	if((sibA == -1) && (sibB == -1)) {
		cost = self;
	}
	if((sibA == -1) && (sibB != -1)) {
		cost = min(self+suf[1][B]-subtree[1][B], subtree[1][B]+dp[A][sibB]);
	}
	if((sibA != -1) && (sibB == -1)) {
		cost = min({cost, self+suf[0][A]-subtree[0][A], subtree[0][A]+dp[sibA][B], subtree[1][B]+suf[0][A]});
	}
	if((sibA != -1) && (sibB != -1)) {
		cost = min({cost, subtree[0][A]+dp[sibA][B], self+dp[sibA][sibB], subtree[1][B]+dp[A][sibB]});
	}
	return cost;
}

void dpTrees(int node) {
	for(int i = int(tree[0][node].size())-1; i >= 0; i --) {
		dpTrees(tree[0][node][i]);
	}
	for(int i = tree[1].size()-1; i >= 1; i --) {
		dp[node][i] = getCost(node, i);
	}
}

void Solve() {
	string T[2];
	cin >> T[0] >> T[1];
	for(int Tree = 0; Tree < 2; Tree ++) {
		tree[Tree].clear();
		label[Tree].clear();
		subtree[Tree].clear();
		nextSibling[Tree].clear();
		suf[Tree].clear();
		int node = 0;
		for(int i = 0; i < T[Tree].size(); i ++) {
			if((T[Tree][i] >= 'A') && (T[Tree][i] <= 'Z')) node ++;
		}
		tree[Tree].resize(node+1);
		label[Tree].resize(node+1);
		subtree[Tree].resize(node+1);
		nextSibling[Tree].assign(node+1, -1);
		suf[Tree].resize(node+1);
		int ID = 1;
		stack<int> st;
		for(int i = 0; i < T[Tree].size(); i ++) {
			if(T[Tree][i] == '(') {
				i ++;
				label[Tree][ID] = T[Tree][i]-'A';
				if(st.size()) {
					int par = st.top();
					tree[Tree][par].push_back(ID);
				}
				st.push(ID);
				ID ++;
			}
			else {
				st.pop();
			}
		}
	}
	dp.assign(tree[0].size()+1, vector<int>(tree[1].size()+1, int(1e6)));
	computeInfo(1, 0);
	computeInfo(1, 1);
	dpTrees(1);
	cout << dp[1][1] << endl;
}

int main() {
	ios_base::sync_with_stdio(0);
	cin.tie(0);
	cout.tie(0);

	int tt = 1;	
	cin >> tt;
	while(tt --) {
		Solve();
	}
	return 0;
}
