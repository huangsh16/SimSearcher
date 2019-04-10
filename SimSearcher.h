#pragma once
#include <vector>
#include <utility>
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <string>
#include <ctime>
#include <fstream>
#include <unordered_map>
#include <set>
using namespace std;

const int SUCCESS = 0;
const int FAILURE = 1;
#define STRSIZE 300
const int MOD = (1 << 19) - 1;
const int INF = (int)1e9;

struct TrieNode{
	TrieNode* child[128];
	vector<int> indexVector;
	TrieNode() {
		for(int i = 0; i < 128; ++i)
			child[i] = NULL;
	}
};

struct Trie
{
	TrieNode* root;
	Trie() { 
		root = new TrieNode(); 
	}
	void Insert(const char* str, int len, int lineId) {
		TrieNode* nowNode = root;
		for (int i = 0; i < len; ++i) {
			if(nowNode -> child[(int)str[i]] == NULL) 
				nowNode -> child[(int)str[i]] = new TrieNode();
			nowNode = nowNode -> child[(int)str[i]];
		}
		if(nowNode -> indexVector.empty() || *(nowNode -> indexVector.end() - 1) != lineId)
			nowNode -> indexVector.push_back(lineId);
	}
	vector<int>* Search(const char* str, int len) {
		TrieNode* nowNode = root;
		for(int i = 0; i < len; ++i) {
			if(nowNode -> child[(int)str[i]] == NULL)
				return NULL;
			nowNode = nowNode -> child[(int)str[i]];
		}
		return &(nowNode -> indexVector);
	}
};

struct Hash {
	unsigned long long v[STRSIZE];
	int next[STRSIZE], q[STRSIZE], h[MOD], tot;
	Hash() {
		tot = 0;
		memset(h, 0, sizeof(h));
	}
	void clear(){
		for(int i = 1; i <= tot; ++i)
			h[q[i]] = 0;
		tot = 0;
	}
	bool Insert(int t1, unsigned long long t2) { // insert succeed return 1 else 0
		for(int i = h[t1]; i; i = next[i])
			if(v[i] == t2)
				return 0;
		v[++tot] = t2; next[tot] = h[t1]; h[t1] = tot;
		q[tot] = t1;
		return 1;
	}
	void Insert(set<unsigned long long> &s) {
		for(auto x : s) {
			int t1 = x & MOD;
			Insert(t1, x);
		}
	}
	bool Search(int t1, unsigned long long t2) {
		for(int i = h[t1]; i; i = next[i])
			if(v[i] == t2)
				return 1;
		return 0;
	}
	int Search(set<unsigned long long> &s) {
		int cnt = 0;
		for(auto x : s) {
			int t1 = x & MOD;
			if(Search(t1, x))
				cnt++;
		}
		return cnt;
	}
};

class SimSearcher
{
public:
	int qGram;
	int lineId;
	Trie edTrie, jaccardTrie;
	int searchTimes;
	int *appearTimes;
	int *isAppear;
	vector<string> strVector;
	vector<int> strSizeVector;
	int dp[STRSIZE][STRSIZE];
	Hash hash;

	int minSetSize;
	vector<int> hashSetSizeVector;
  	vector<set<unsigned long long> > hashSetVector;
	vector<vector<int>* > indexVectorVector;
	vector<pair<int, int> > indexVectorLen;

	inline int GetJaccardThreshold(double threshold, int queryNum, int lineNum);
	double ComputeJaccard(std::set<unsigned long long> &l1, std::set<unsigned long long> &l2, double threshold);

	void InitIndexVectorVectorWithEdTrie(const char *query, int len);
	void InitIndexVectorVectorWithJaccardTrie(set<string> Set);
	int ComputeEd(const char* str1, int m, const char* str2, int n, int threshold);

	SimSearcher();
	~SimSearcher();

	int createIndex(const char *filename, unsigned q);
	int searchJaccard(const char *query, double threshold, std::vector<std::pair<unsigned, double> > &result);
	int searchED(const char *query, unsigned threshold, std::vector<std::pair<unsigned, unsigned> > &result);
};

