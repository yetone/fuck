#include <string>
#include <vector>

#include "code.h"

using namespace std;

#ifndef H_METHOD
#define H_METHOD

class Method {
public:
	struct Chunk chunk;
	const string ns;
	const string name;
	vector<string> lines;
	goto_labels labels;

	Method(string name);
	Method(string ns, string name);
	Method(string ns, string name, vector<string>& lines);
	vector<string>& getlines();
	const string& getname();
	const string& getnamespace();
	const string getdisplayname();
};

#endif
