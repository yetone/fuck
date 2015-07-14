#include <fstream>

#include "code.h"

using namespace std;

Code::Code(string file): file(file) {
	list<string> lines;

	ifstream input(file.c_str());

	string line;
	while (getline(input, line)) {
		lines.push_back(line);
	}

	this->lines = lines;
}

Code::Code(string file, list<string> lines): file(file), lines(lines) {

}
