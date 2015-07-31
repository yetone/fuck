#include <fstream>
#include <vector>

#include "headers/code.h"
#include "headers/utils.h"

using namespace std;

Code::Code(const string& file): file(file) {
	vector<string> lines;

	ifstream input(file.c_str());

	string line;
	while (getline(input, line)) {
		lines.push_back(line);
	}

	this->lines = lines;
}

Code::Code(const string& file, vector<string>& lines): file(file), lines(lines) {

}

vector<string>& Code::getlines() {
	return this->lines;
}

const string& Code::getfile() {
	return this->file;
}
