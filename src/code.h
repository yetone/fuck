#include <list>
#include <string>
#include <vector>

using namespace std;

#ifndef H_CODE
#define H_CODE

class Code {
private:
	const string file;
	vector<string> lines;
public:
	/*
	 * Loads line list from file
	 */
	Code(string file);
	Code(string file, vector<string> lines);

	vector<string> getlines();
};

#endif
