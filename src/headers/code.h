#include <list>
#include <string>
#include <vector>

using namespace std;

typedef pair<string, int> goto_pair;
typedef vector<goto_pair> goto_labels;

#ifndef H_CODE
#define H_CODE

class Code {
public:
	string file;
	vector<string> lines;
	/*
	 * Loads line list from file
	 */
	Code(string file);
	Code(string file, vector<string> lines);

	vector<string> getlines();
	string getfile();
};

template <typename T>
class Variable {
public:
	T var;
	string name;
};

#endif

typedef Variable<string> defvar;
typedef vector<defvar> stack;

