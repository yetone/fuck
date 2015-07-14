#include <list>
#include <string>

using namespace std;

class Code {
private:
	const string file;
	list<string> lines;
public:
	/*
	 * Loads line list from file
	 */
	Code(string file);
	Code(string file, list<string> lines);

	list<string> getlines();
};
