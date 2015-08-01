#include <fstream>
#include <vector>

#include "headers/code.h"
#include "headers/utils.h"

using namespace std;

Code::Code(const string& file): file(file) {
	vector<wstring> lines;

	wifstream input(file);

	wstring line;
	while (getline(input, line)) {
		lines.push_back(line);
	}

	this->lines = lines;
}

Code::Code(const string& file, vector<wstring>& lines): file(file), lines(lines) {

}

vector<wstring>& Code::getlines() {
	return this->lines;
}

const string& Code::getfile() {
	return this->file;
}
