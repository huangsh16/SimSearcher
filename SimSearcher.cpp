#include "SimSearcher.h"
#include <cstdlib>

SimSearcher::SimSearcher()
{
}

SimSearcher::~SimSearcher()
{
}

#define Abs(_) ((_) > 0 ? (_) : -(_))
#define Ok(a, b, threshold) ((a) > (b) ? ((b) >= (a) * threshold) : ((a) >= (b) * threshold))
#define CountNeedTimes(a, b, threshold) (ceil(((a) + (b)) * (threshold) / (1 + (threshold))))

unsigned long long HashValue(const char *str, int len) {
    unsigned long long ans = 0;
    for(int i = 0; i < len; ++i)
        ans = ans * 131 + str[i];
    return ans;
}

int SimSearcher::createIndex(const char *filename, unsigned q)
{
	ifstream fin(filename);
    string strString; lineId = 0; qGram = q; minSetSize = INF;
	char str[STRSIZE];
	int len;
	while(getline(fin, strString)) {
		len = strString.length();
		strString.copy(str, len, 0);
		*(str + len) = '\0';
		
        for(int i = 0; i + qGram - 1 < len; ++i) {
			edTrie.Insert(str + i, qGram, lineId);
		}

        int last = 0; set<unsigned long long> hashSet;
        for(int i = 0; i < len; ++i)
            if(str[i] == ' ') {
                if(last < i) {
                    jaccardTrie.Insert(str + last, i - last, lineId);
                    hashSet.insert(HashValue(str + last, i - last));
                }
                last = i + 1;
            }
        if(last < len) {
            jaccardTrie.Insert(str + last, len - last, lineId);
            hashSet.insert(HashValue(str + last, len - last));
        }
        hashSetVector.push_back(hashSet);
        hashSetSizeVector.push_back(hashSet.size());
        minSetSize = min(minSetSize, (int)hashSet.size());
		lineId++;
		strVector.push_back(str);
		strSizeVector.push_back(len);
	};
	
	searchTimes = 0;
	appearTimes = new int[lineId];
	isAppear = new int[lineId];
    memset(isAppear, 0, sizeof(int) * lineId);
	return SUCCESS;
}

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

void SimSearcher::InitIndexVectorVectorWithJaccardTrie(set<string> Set) {
    indexVectorVector.clear();
    indexVectorLen.clear();
    int id = 0;
    for(auto element : Set) {
        vector<int> *tmp = jaccardTrie.Search(element.c_str(), element.length());
        if(tmp) {
            indexVectorVector.push_back(tmp);
            indexVectorLen.push_back(make_pair(tmp -> size(), id++));
        }
    }
    sort(indexVectorLen.begin(), indexVectorLen.end());   
}

inline int SimSearcher::GetJaccardThreshold(double threshold, int queryNum, int lineNum)
{
    double t1 = threshold * queryNum;
    double t2 = (queryNum + lineNum) * threshold / (1 + threshold);
    return ceil(t1) > ceil(t2) ? ceil(t1) : ceil(t2);
}

double SimSearcher::ComputeJaccard(set<unsigned long long> &l1, set<unsigned long long> &l2, double threshold)
{
    int cnt = 0;
    for (auto w : l1)
    {
        if (l2.find(w) != l2.end())
            cnt++;
    }
    return ((double)cnt / (double)(l1.size() + l2.size() - cnt));
}

int SimSearcher::searchJaccard(const char *query, double threshold, vector<pair<unsigned, double> > &result)
{
	result.clear();
    searchTimes++;
    string queryString(query);
    int query_len = strlen(query), last = 0;
    set<unsigned long long> queryHashSet;
    set<string> queryWordSet;
    for(int i = 0; i < query_len; ++i)
        if(query[i] == ' ') {
            if(last < i) {
                queryWordSet.insert(queryString.substr(last, i - last));
                queryHashSet.insert(HashValue(query + last, i - last));
            }
            last = i + 1;
        }
    if(last < query_len) {
        queryWordSet.insert(queryString.substr(last, query_len - last));
        queryHashSet.insert(HashValue(query + last, query_len - last));
    }
    int needTimes = GetJaccardThreshold(threshold, queryWordSet.size(), minSetSize);
    vector<int> shortVector, tmpVector, ansVector;

    if(needTimes > 0) {
        InitIndexVectorVectorWithJaccardTrie(queryWordSet);
        int indexVectorVectorSize = indexVectorVector.size();
        for(int i = indexVectorVectorSize - needTimes; i >= 0; --i)
            for(auto index : *indexVectorVector[indexVectorLen[i].second]) {
                if(isAppear[index] != searchTimes) {
                    isAppear[index] = searchTimes;
                    appearTimes[index] = 0;
                    shortVector.push_back(index);
                }
                appearTimes[index]++;
            }
        for(auto index : shortVector) {
            if(!Ok(hashSetSizeVector[index], queryWordSet.size(), threshold))
                continue;
            int needTimesOfStr = CountNeedTimes(hashSetSizeVector[index], queryWordSet.size(), threshold);
            int appearTimesCnt = appearTimes[index];
            if(appearTimesCnt + needTimes - 1 < needTimesOfStr)
                continue;
            tmpVector.push_back(index);
        }

        double sumLog; int sum = 0;
        for(int i = indexVectorVectorSize - needTimes + 1; i < indexVectorVectorSize; ++i)
            sum += indexVectorLen[i].first, sumLog += log2(indexVectorLen[i].first);


        if(tmpVector.size() * sumLog < sum * 1.5) {
            for(auto index : tmpVector) {
                int appearTimesCnt = appearTimes[index];
                int needTimesOfStr = CountNeedTimes(hashSetSizeVector[index], queryWordSet.size(), threshold);
                needTimesOfStr = max(needTimesOfStr, needTimes);
                for(int i = indexVectorVectorSize - needTimes + 1; i < indexVectorVectorSize; ++i) {
                    if(appearTimesCnt >= needTimesOfStr || appearTimesCnt + indexVectorVectorSize - i < needTimesOfStr)
                        break;
                    if(binary_search(indexVectorVector[indexVectorLen[i].second] -> begin(), indexVectorVector[indexVectorLen[i].second] -> end(), index))
                        appearTimesCnt++;
                }
                if(appearTimesCnt >= needTimesOfStr)
                    ansVector.push_back(index);
            }
        }
        else {
            for(int i = indexVectorVectorSize - needTimes + 1; i < indexVectorVectorSize; ++i)
                for(auto index : *indexVectorVector[indexVectorLen[i].second])
                    appearTimes[index]++;
            for(auto index : tmpVector) {
                int needTimesOfStr = CountNeedTimes(hashSetSizeVector[index], queryWordSet.size(), threshold);
                needTimesOfStr = max(needTimesOfStr, needTimes);
                if(appearTimes[index] >= needTimesOfStr)
                    ansVector.push_back(index);
            }
        }   
        sort(ansVector.begin(), ansVector.end());
    }
    else {
        for(int i = 0; i < lineId; ++i)
            if(Ok(hashSetSizeVector[i], queryWordSet.size(), threshold))
                ansVector.push_back(i);   
    }

    hash.Insert(queryHashSet);
    for(auto index : ansVector) { 
		//double jaccard = ComputeJaccard(queryHashSet, hashSetVector[index], threshold);
		int cnt = hash.Search(hashSetVector[index]);
        double jaccard = ((double)cnt / (double)(queryHashSet.size() + hashSetVector[index].size() - cnt));
        if(jaccard >= threshold)
            result.push_back(make_pair(index, jaccard));
    }
    hash.clear();
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
    int needTimes = query_len - qGram + 1 - (int)thresholdInt * qGram;
    vector<int> shortVector, tmpVector, ansVector;
	
    if(needTimes > 0) {
		InitIndexVectorVectorWithEdTrie(query, query_len);
    	int indexVectorVectorSize = indexVectorVector.size();
    	for(int i = indexVectorVectorSize - needTimes; i >= 0; --i)
    		for(auto index : *indexVectorVector[indexVectorLen[i].second]) {
    			if(isAppear[index] != searchTimes) {
    				isAppear[index] = searchTimes;
    				appearTimes[index] = 0;
    				shortVector.push_back(index);
    			}
    			appearTimes[index]++;
    		}
    	for(auto index : shortVector) {
    		if(Abs(strSizeVector[index] - query_len) > thresholdInt)
    			continue;
    		int appearTimesCnt = appearTimes[index];
    		int needTimesOfStr = query_len - qGram + 1 - thresholdInt * qGram;
    		if(appearTimesCnt + needTimes - 1 < needTimesOfStr)
    			continue;
    		tmpVector.push_back(index);
    	}
        double sumLog; int sum = 0;
        for(int i = indexVectorVectorSize - needTimes + 1; i < indexVectorVectorSize; ++i)
            sum += indexVectorLen[i].first, sumLog += log2(indexVectorLen[i].first);

        if(tmpVector.size() * sumLog < sum * 2.5) {
            for(auto index : tmpVector) {
                int appearTimesCnt = appearTimes[index];
                for(int i = indexVectorVectorSize - needTimes + 1; i < indexVectorVectorSize; ++i) {
                    if(appearTimesCnt >= needTimes || appearTimesCnt + indexVectorVectorSize - i < needTimes)
                        break;
                    if(binary_search(indexVectorVector[indexVectorLen[i].second] -> begin(), indexVectorVector[indexVectorLen[i].second] -> end(), index))
                        appearTimesCnt++;
                }
                if(appearTimesCnt >= needTimes)
                    ansVector.push_back(index);
            }
        }
        else {
            for(int i = indexVectorVectorSize - needTimes + 1; i < indexVectorVectorSize; ++i)
                for(auto index : *indexVectorVector[indexVectorLen[i].second])
                    appearTimes[index]++;
            for(auto index : tmpVector)
                if(appearTimes[index] >= needTimes)
                    ansVector.push_back(index);
        }   
    	sort(ansVector.begin(), ansVector.end());
    } 
    else {
    	for(int i = 0; i < lineId; ++i)
    		ansVector.push_back(i);
    }
    
    for(auto index : ansVector) { 
        int ed = ComputeEd(strVector[index].c_str(), strSizeVector[index], query, query_len, thresholdInt);
    	if(ed <= thresholdInt)
    		result.push_back(make_pair(index, ed));
    }
	return SUCCESS;
}

