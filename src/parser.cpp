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

extern methodmap methodMap;

void parse(Code *code) {
	printverbose("Parsing " + code->getfile());

	vector<string> lines = code->getlines();

	// Current namespace, might be empty
	string currentns = "";

	// Current method name, if no method, should be "main"
	Method *currentmethod;
	Method main(ENTRY_POINT);
	methodMap.push_back(&main);
	currentmethod = &main;

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

		printverbose("Keyword: " + keyword + "Line: " + line);

		if (keyword == KW_NAMESPACE) {
			if (end) {
				currentns = "";
			} else {
				currentns = line;
			}
		} else if (keyword == KW_METHOD) {
			if (end) {
				printverbose("Method finished: " + currentmethod->getdisplayname());
				methodMap.push_back(currentmethod);

				currentmethod = &main;
			} else {
				currentmethod = new Method(currentns, line);
				printverbose("Method initialized: " + currentmethod->getdisplayname());
			}
		} else {
			currentmethod->lines.push_back(keyword + " " + line);
		}
	}

	invoke();
}

void invoke() {
	invoke(ENTRY_POINT);
}

void invoke(string s) {
	Method* method;

	for (unsigned int i = 0; i < methodMap.size(); i++) {
		Method *m = methodMap.at(i);

		if (m->getdisplayname() == s) {
			method = m;
			break;
		}
	}

	invoke(method);
}

void invoke(Method* method) {
	printverbose("Invoking " + method->getdisplayname());

	vector<string> lines = method->getlines();

	for (unsigned int i = 0; i < lines.size(); i++) {
		string line = lines[i];

		unsigned int firstsep = line.find_first_of(KEYWORD_SEPARATOR);
		string keyword = firstsep == string::npos ? line : line.substr(0, firstsep);
		line = line.substr(firstsep + 1, line.length());

		printverbose("Executing keyword: " + keyword + ", line: " + line);

		if (keyword == KW_CALL_METHOD) {
			invoke(line);
		} else if (keyword == KW_PRINT) {
			cout << line << endl;
		}
	}
}
