#include "code.h"

Code::Code(string file): file(file) {
	list<string> lines;

	ifstream input(file);

	string line;
	while (getline(input, line)) {
		lines.push_back(line);
	}

	this->lines = lines;
}

Code::Code(string file, list<string> lines): file(file), lines(lines) {

}
