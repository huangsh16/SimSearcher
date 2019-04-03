#include "SimSearcher.h"
#include <cstdlib>

SimSearcher::SimSearcher()
{
}

SimSearcher::~SimSearcher()
{
}

#define read() do{ if (pt >= pend) { pt = buf; readSize = fread(buf, 1, BUFSIZE, file); } } while(0)
#define Scan_str(s) { len = 0; read(); while(*pt == '\n' || *pt == '\r'){pt++; read();} while (*pt != '\n' && *pt != '\r' && (pt - buf < readSize)) {s[len++] = (*(pt ++)); read();} s[len] = '\0'; }
#define Abs(_) ((_) > 0 ? (_) : -(_))

int SimSearcher::createIndex(const char *filename, unsigned q)
{
	time_t st = clock();
	ifstream fin(filename);
    string str;
	lineId = 0; qGram = q;
	char str1[STRSIZE];
	int len;
	while(getline(fin, str)) {
		len = str.length();
		str.copy(str1, len, 0);
		*(str1 + len) = '\0';
		for(int i = 0; i + qGram - 1 < len; ++i) {
			edTrie.Insert(str1 + i, qGram, lineId);
		}
		lineId++;
		strVector.push_back(str);
		strSizeVector.push_back(len);
		//cout << lineId  << " " << str << endl;
	};
	cout << "readtime : " << clock() - st << endl;
	//cout << lineId << endl;
	searchTimes = 0;
	appearTimes = new int[lineId];
	isAppear = new int[lineId];
    memset(isAppear, 0, sizeof(int) * lineId);
	return SUCCESS;
}

/*
int SimSearcher::createIndex(const char *filename, unsigned q)
{
	FILE *file = fopen(filename, "rb");
	if(file == NULL) return FAILURE;
	buf = new char[BUFSIZE];
	char *pt = buf + BUFSIZE, *pend = buf + BUFSIZE, str[STRSIZE];
	int len, readSize;
	lineId = 0; qGram = q;
	while(1) {
		Scan_str(str);
		cout << lineId  << " " << str << endl;
		if(len == 0) 
			break;
		for(int i = 0; i + qGram - 1 < len; ++i) {
			edTrie.Insert(str + i, qGram, lineId);
		}
		lineId++;
		strVector.push_back(str);
		strSizeVector.push_back(len);
        if(pt - buf >= readSize && readSize != BUFSIZE)
            break;
	};
	cout << lineId << endl;
	searchTimes = 0;
	appearTimes = new int[lineId];
	isAppear = new int[lineId];
    memset(isAppear, 0, sizeof(int) * lineId);
    fclose(file);
	return SUCCESS;
}*/

/*
int SimSearcher::createIndex(const char *filename, unsigned q)
{
	FILE *file = fopen(filename, "r+");
	if(file == NULL) return FAILURE;
	char *pt = buf + BUFSIZE, *pend = buf + BUFSIZE, str[STRSIZE];
	int len, readSize;
	lineId = 0; qGram = q;
	while(fgets(str, 260, file) != NULL) {
		len = strlen(str);
		if(str[len - 1] == '\n')
			str[--len] = '\0';
		for(int i = 0; i + qGram - 1 < len; ++i) {
			edTrie.Insert(str + i, qGram, lineId);
		}
		lineId++;
		strVector.push_back(str);
		strSizeVector.push_back(len);
		cout << lineId << " " << str << endl;
	};
	cout << lineId << endl;
	searchTimes = 0;
	appearTimes = new int[lineId];
	isAppear = new int[lineId];
    memset(isAppear, 0, sizeof(int) * lineId);
	return SUCCESS;
}*/


void SimSearcher::InitIndexVectorVectorWithEdTrie(const char *str, int len) {
	indexVectorVector.clear();
	indexVectorLen.clear();
	for(int i = 0, id = 0; i + qGram - 1 < len; ++i) {
		vector<int>* tmp = edTrie.Search(str + i, qGram);
		if(tmp) {
			indexVectorVector.push_back(tmp);
			indexVectorLen.push_back(make_pair(tmp -> size(), id++));
		}
	}
	sort(indexVectorLen.begin(), indexVectorLen.end());
}

int SimSearcher::searchJaccard(const char *query, double threshold, vector<pair<unsigned, double> > &result)
{
	result.clear();
	return SUCCESS;
}

int SimSearcher::ComputeEd(const char* str1, int m, const char* str2, int n, int threshold)
{
    if (Abs(m - n) > threshold)
        return INF;
    for(int i = 0; i <= min(threshold, m); ++i) dp[i][0] = i;
    for(int j = 0; j <= min(threshold, n); ++j) dp[0][j] = j;
    for(int i = 1; i <= m; i++) {
        int begin = max(i - threshold, 1);
        int end = min(i + threshold, n);
        if (begin > end)
            break;
        for (int j = begin; j <= end; j++) {
            int t = !(str1[i - 1] == str2[j - 1]);
            int d1 = Abs(i - 1 - j) > threshold ? INF : dp[i - 1][j];
            int d2 = Abs(i - j + 1) > threshold ? INF : dp[i][j - 1];
            dp[i][j] = min(dp[i - 1][j - 1] + t, min(d1 + 1, d2 + 1));
        }
    }
    return dp[m][n];
}

int SimSearcher::searchED(const char *query, unsigned threshold, vector<pair<unsigned, unsigned> > &result)
{
	result.clear();
	searchTimes++;
	int query_len = strlen(query);
    int thresholdInt = threshold > 1000000000u ? (int)1e9 : threshold;
    //cout << thresholdInt << endl;
    int needTimes = query_len - qGram + 1 - (int)thresholdInt * qGram;
    vector<int> shortVector, ansVector;
	//cout << query << endl;
	//cout << query_len << " " << qGram << " " << thresholdInt << endl;
    //cout << needTimes << endl;

    if(needTimes > 0) {
		InitIndexVectorVectorWithEdTrie(query, query_len);
    	/*
    	for(auto indexVector : indexVectorVector) {
    		for(auto index : *indexVector)
    			cout << index << " ";
    		cout << endl;
    	}*/
    	int indexVectorVectorSize = indexVectorVector.size();
    	//cout << indexVectorVectorSize << endl;
    	for(int i = indexVectorVectorSize - needTimes; i >= 0; --i) {
    		//cout << i << ":";
    		for(auto index : *indexVectorVector[indexVectorLen[i].second]) {
    			//cout << index << " ";
    			if(isAppear[index] != searchTimes) {
    				isAppear[index] = searchTimes;
    				appearTimes[index] = 0;
    				shortVector.push_back(index);
    			}
    			appearTimes[index]++;
    		}
    		//cout << endl;
    	}
    	for(auto index : shortVector) {
    		//cout << "index " << index << endl;
    		if(Abs(strSizeVector[index] - query_len) > thresholdInt)
    			continue;
    		int appearTimesCnt = appearTimes[index];
    		int needTimesOfStr = query_len - qGram + 1 - thresholdInt * qGram;
    		if(appearTimesCnt + needTimes - 1 < needTimesOfStr)
    			continue;
    		for(int i = indexVectorVectorSize - needTimes + 1; i < indexVectorVectorSize; ++i) {
    			if(appearTimesCnt >= needTimes || appearTimesCnt + indexVectorVectorSize - i < needTimes)
    				break;
    			if(binary_search(indexVectorVector[indexVectorLen[i].second] -> begin(), indexVectorVector[indexVectorLen[i].second] -> end(), index))
    				appearTimesCnt++;
    		}
    		if(appearTimesCnt >= needTimes)
    			ansVector.push_back(index);
    	}
    	sort(ansVector.begin(), ansVector.end());
    } 
    else {
    	/*
    	for(auto indexVector: indexVectorVector) 
    		for(auto index : *indexVector) {
    			if(isAppear[index] != searchTimes) {
    				isAppear[index] = searchTimes;
    				ansVector.push_back(index);
    			}
    		}
    	*/
    	for(int i = 0; i < lineId; ++i)
    		ansVector.push_back(i);
    }
    
    for(auto index : ansVector) { 
    	//cout << "no " << index << endl; 
    	int ed = ComputeEd(strVector[index].c_str(), strSizeVector[index], query, query_len, thresholdInt);
    	//if(ed == 2 && index == 14)
    	//	cout << strVector[index].c_str() << " " << query << endl;
    	if(ed <= thresholdInt)
    		result.push_back(make_pair(index, ed));
    }
	return SUCCESS;
}

