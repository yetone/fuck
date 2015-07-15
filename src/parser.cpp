#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <map>

#include "parser.h"
#include "keywords.h"
#include "code.h"
#include "utils.h"

using namespace std;

extern bool verbose;

methodmap* parse(Code *code) {
	methodmap map;
	vector<string> lines = code->getlines();

	// Current namespace, might be empty
	string currentns = "";

	// Current method name, if no method, should be "main"
	Method *currentmethod;
	Method main(ENTRY_POINT);
	map.push_back(&main);

	for (unsigned int i = 0; i < lines.size(); i++) {
		string line = lines[i];

		line = trim(line);

		bool end = startswith(line, KW_END);

		if (end) {
			line = line.substr(strlen(KW_END), line.size());
		}

		unsigned int firstsep = line.find_first_of(KEYWORD_SEPARATOR);
		string keyword = firstsep == string::npos ? line : line.substr(0, firstsep);
		line = line.substr(firstsep + 1, line.length());

		if (verbose) {
			cout << "Keyword: "  << keyword << endl << "Line: " << line << endl << endl;
		}

		if (keyword == KW_NAMESPACE) {
			if (end) {
				currentns = line;
			} else {
				currentns = "";
			}
		} else if (keyword == KW_METHOD) {
			if (end) {
				printverbose("Method finished: " + currentmethod->getdisplayname());
				map.push_back(currentmethod);

				currentmethod = &main;
			} else {
				currentmethod = new Method(line);
				printverbose("Method initialized: " + currentmethod->getdisplayname());
			}
		} else {
			currentmethod->getlines().push_back(line);
		}
	}

	return &map;
}

void run(methodmap* map) {
	Method* main;

	for (unsigned int i = 0; i < map->size(); i++) {
		Method m = map[i];
		if (m.getname() == ENTRY_POINT) {
			main = &m;
			break;
		}
	}

	//cout << "what " << main->getdisplayname() << endl;

	//run(method);
}

void run(Method* method) {
	if (verbose) {
		cout << "Running " << method->getdisplayname() << endl;
	}

	vector<string> lines = method->getlines();

	for (unsigned int i = 0; i < lines.size(); i++) {
		string line = lines[i];

		unsigned int firstsep = line.find_first_of(KEYWORD_SEPARATOR);
		string keyword = firstsep == string::npos ? line : line.substr(0, firstsep);
		line = line.substr(firstsep + 1, line.length());

		cout << "Running keyword: " << keyword << ", line: " << line << endl;
	}
}
