#include "SimSearcher.h"
#include <cstdlib>

SimSearcher::SimSearcher()
{
}

SimSearcher::~SimSearcher()
{
}

#define read() do{ if (pt >= pend) { pt = buf; fread(buf, 1, BUFSIZE, file); } } while(0)
#define scan_str(s) { len = 0; read(); while (*pt != '\n') {s[len++] = (*(pt ++)); read();} pt++; s[len] = '\0'; }

int SimSearcher::createIndex(const char *filename, unsigned q)
{
	FILE *file = fopen(filename, "r+");
	if(file == NULL) return FAILURE;
	char buf[BUFSIZE], *pt = buf + BUFSIZE, *pend = buf + BUFSIZE, str[STRSIZE];
	int lineId = 0, qGram = q, len;
	do {
		scan_str(str);
		//printf("%d %s\n", lineId, str);
		if(!len) {
			for(int i = 0; i + qGram - 1 < len; ++i) {
				edTrie.Insert(str + i, qGram, lineId);
			}
			lineId++;
		}
	}while(!len);
	return SUCCESS;
}

int SimSearcher::searchJaccard(const char *query, double threshold, vector<pair<unsigned, double> > &result)
{
	result.clear();
	return SUCCESS;
}

int SimSearcher::searchED(const char *query, unsigned threshold, vector<pair<unsigned, unsigned> > &result)
{
	result.clear();
	return SUCCESS;
}

