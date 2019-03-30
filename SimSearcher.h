#pragma once
#include <vector>
#include <utility>
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
using namespace std;

const int SUCCESS = 0;
const int FAILURE = 1;
#define BUFSIZE 1000000

char buf[BUFSIZE], *pt = buf + BUFSIZE, *pend = buf + BUFSIZE;
#define read() \
do{ \
if (pt >= pend) { \
pt = buf; fread(buf, 1, BUFSIZE, file); } } while(0)


#define scan_str(s) \
{ int p = 0; \
read(); \
//while (*pt < '0' || *pt > '9') {pt++; read();} \
while (*pt != '\n') {s[p++] = (*(pt ++)); read();}  \
s[p] = '\0'; }

struct TrieNode{
	TrieNode* child[128];
	vector<int> indexVector;
	TrieNode() {}
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
	Trie edTrie;
	SimSearcher();
	~SimSearcher();

	int createIndex(const char *filename, unsigned q);
	int searchJaccard(const char *query, double threshold, std::vector<std::pair<unsigned, double> > &result);
	int searchED(const char *query, unsigned threshold, std::vector<std::pair<unsigned, unsigned> > &result);
};

