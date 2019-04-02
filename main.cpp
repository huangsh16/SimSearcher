#include "SimSearcher.h"

using namespace std;

int main(int argc, char **argv)
{
	SimSearcher searcher;

	vector<pair<unsigned, unsigned> > resultED;
	vector<pair<unsigned, double> > resultJaccard;

	unsigned q = 1, edThreshold = 4;
	double jaccardThreshold = 0.85;

	searcher.createIndex(argv[1], q);

	freopen(argv[2], "r", stdin);
	freopen(argv[3], "w", stdout);

	string str;
	int t;
	int id;
	while(cin >> str >> t) {
		searcher.searchED(str.c_str(), (unsigned)t, resultED);
		printf("%d:\n", id++);
		for(auto result : resultED)
			printf("%d %d\n", result.first, result.second);
	}

/*
	searcher.createIndex(argv[1], q);
	searcher.searchJaccard("query", jaccardThreshold, resultJaccard);
	searcher.searchED("abcabcabc", edThreshold, resultED);
	searcher.searchED("abcabcabc", edThreshold, resultED);
	searcher.searchED("abcabcabc", edThreshold, resultED);
	searcher.searchED("abcabcabc", edThreshold, resultED);

	cout << "resultED : " << endl;

	for(auto result : resultED)
		cout << result.first << " " << result.second << endl;

	cout << "resultJaccard :" << endl;

	for(auto result : resultJaccard)
		cout << result.first << " " << result.second << endl;
*/
	return 0;
}

