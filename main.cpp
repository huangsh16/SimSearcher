#include "SimSearcher.h"

using namespace std;

int main(int argc, char **argv)
{
	time_t st = clock();
	SimSearcher searcher;

	vector<pair<unsigned, unsigned> > resultED;
	vector<pair<unsigned, double> > resultJaccard;

	unsigned q = 1, edThreshold = 4;
	double jaccardThreshold = 0.2;

	//searcher.createIndex(argv[1], q);

/*
	freopen(argv[2], "r", stdin);
	//freopen(argv[3], "w", stdout);

	string str;
	int t;
	int id = 0;
	//cout << id++ << endl;
	while(cin >> str >> t) {
		//cout << str.length() << " " << t << endl;
		searcher.searchED(str.c_str(), (unsigned)t, resultED);
		//cout << id++ << endl;
		//printf("%d:\n", id++);
		//for(auto result : resultED)
		//	printf("%d %d\n", result.first, result.second);
	}
	fclose(stdin);
	//fclose(stdout);

	cout << "alltime : " << clock() - st << endl;
*/

//	searcher.createIndex(argv[1], q);
	//searcher.searchJaccard("query", jaccardThreshold, resultJaccard);

//	cout << "resultED : " << endl;

//	for(auto result : resultED)
//		cout << result.first << " " << result.second << endl;

	cout << "resultJaccard :" << endl;

	for(auto result : resultJaccard)
		cout << result.first << " " << result.second << endl;

	return 0;
}

