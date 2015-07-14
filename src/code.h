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

class Method {
private:
	const string ns;
	const string name;
	vector<string> lines;

public:
	Method(string ns, string name);
	Method(string ns, string name, vector<string> lines);
	vector<string> getlines();
	string getname();
	string getnamespace();
	string getdisplayname();
};

#endif
