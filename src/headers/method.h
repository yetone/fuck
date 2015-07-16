#include <string>
#include <vector>

#include "code.h"

using namespace std;

class Method {
public:
	string ns;
	string name;
	vector<string> lines;
	goto_labels goto_labels;

	Method(string name);
	Method(string ns, string name);
	Method(string ns, string name, vector<string> lines);
	vector<string> getlines();
	string getname();
	string getnamespace();
	string getdisplayname();
};
