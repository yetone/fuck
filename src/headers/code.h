#include <list>
#include <string>
#include <vector>

using namespace std;

typedef pair<wstring, int> goto_pair;
typedef vector<goto_pair> goto_labels;

#ifndef H_CODE
#define H_CODE

#define DEFAULT_COND false

struct Chunk {
	int start = 0;
	int end = 0;
};

class Code {
public:
	const string file;
	vector<wstring> lines;
	/*
	 * Loads line list from file
	 */
	Code(const string& file);
	Code(const string& file, vector<wstring>& lines);

	vector<wstring>& getlines();
	const string& getfile();
};

#endif
