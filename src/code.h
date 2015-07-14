#include <list>
#include <string>
#include <vector>

using namespace std;

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
