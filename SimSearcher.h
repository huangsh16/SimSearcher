#pragma once
#include <vector>
#include <utility>
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <string>
using namespace std;

const int SUCCESS = 0;
const int FAILURE = 1;
#define BUFSIZE 1000000
#define STRSIZE 300
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
			if(nowNode -> child[str[i]] == NULL) 
				nowNode -> child[str[i]] = new TrieNode();
			nowNode = nowNode -> child[str[i]];
		}
		if(nowNode -> indexVector.empty() || *(nowNode -> indexVector.end() - 1) != lineId)
			nowNode -> indexVector.push_back(lineId);
	}
	vector<int>* Search(const char* str, int len) {
		TrieNode* nowNode = root;
		for(int i = 0; i < len; ++i) {
			if(nowNode -> child[str[i]] == NULL)
				return NULL;
			nowNode = nowNode -> child[str[i]];
		}
		return &(nowNode -> indexVector);
	}
};

class SimSearcher
{
public:
	int qGram;
	int lineId;
	Trie edTrie;
	int searchTimes;
	int *appearTimes;
	int *isAppear;
	char *buf;
	vector<string> strVector;
	vector<int> strSizeVector;
	int dp[STRSIZE][STRSIZE];

	vector<vector<int>* > indexVectorVector;
	vector<pair<int, int> > indexVectorLen;

	void InitIndexVectorVectorWithEdTrie(const char *query, int len);
	int ComputeEd(const char* str1, int m, const char* str2, int n, int threshold);

	SimSearcher();
	~SimSearcher();

	int createIndex(const char *filename, unsigned q);
	int searchJaccard(const char *query, double threshold, std::vector<std::pair<unsigned, double> > &result);
	int searchED(const char *query, unsigned threshold, std::vector<std::pair<unsigned, unsigned> > &result);
};

