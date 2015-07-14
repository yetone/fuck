#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <cstring>

#include "parser.h"
#include "keywords.h"
#include "code.h"
#include "utils.h"

using namespace std;

void parse(Code *code) {
	vector<string> lines = code->getlines();

	string currentns = "";

	for (unsigned int i = 0; i < lines.size(); i++) {
		string line = lines[i];

		bool end = startswith(line, KW_END);

		if (end) {
			line = line.substr(strlen(KW_END), line.size());
		}

		unsigned int firstsep = line.find_first_of(KEYWORD_SEPARATOR);
		string keyword = firstsep == string::npos ? line : line.substr(0, firstsep + 1);
		line = line.substr(firstsep + 1, line.length());

		cout << "Keyword: "  << keyword << " Line: " << line << endl;

		if (startswith(line, KW_NAMESPACE)) {
			if (end) {
				currentns = removekeyword(line);
			} else {
				currentns = "";
			}
		}
	}
}

string removekeyword(string line) {
	return line;
}
