#include <fstream>
#include <iostream>
#include <vector>

#include "code.h"

using namespace std;

void parse(Code *code) {
	vector<string> lines = code->getlines();

	for (unsigned int i = 0; i < lines.size(); i++) {
		string line = lines[i];
		std::cout << line << endl;
	}
}
