#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <map>
#include <stdlib.h>

#include "parser.h"
#include "keywords.h"
#include "code.h"
#include "utils.h"

using namespace std;

extern methodmap methodMap;
extern stack stackMap;

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

		if (is_comment(line)) {
			continue;
		}

		bool end = startswith(line, KW_END);

		if (end) {
			line = line.substr(strlen(KW_END), line.size());
		}

		if (is_label(line)) {
			// Get label from line, remove KW_GOTO_LABEL in beginning
			string label = line.substr(1, line.length());

			// Add pair to goto_labels list
			currentmethod->goto_labels.push_back(make_pair(label, currentmethod->lines.size() - 1));
			continue;
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
			cout << parsevars(line) << endl;
		} else if (keyword == KW_PRINT_ERR) {
			err(line);
		} else if (keyword == KW_GOTO) {
			goto_labels goto_lines = method->goto_labels;
			for (unsigned int s = 0; s < goto_lines.size(); s++) {
				goto_pair pair = goto_lines[s];

				if (pair.first == line) {
					i = pair.second;
					continue;
				}
			}
		} else if (keyword == KW_SET_VAR) {
			// get type
			int f = line.find_first_of(" ");
			string type = line.substr(0, f);

			// get everything else in line that the variable should be set to
			int f2 = line.find(" ", f + 1);
			string name = line.substr(f + 1, f2 - f - 1);

			string value = line.substr(line.find(" ", f2) + 1);

			printverbose("Setting variable \"" + name + "\" typeof(" + type + ") to \"" + value + "\"");

			defvar var;
			var.name = name;
			var.var = value;
			stackMap.push_back(var);
		}
	}
}

inline bool is_label(string s) {
	return startswith(s, KW_GOTO_LABEL);
}

inline bool is_comment(string s) {
	return startswith(s, COMMENT_1) || startswith(s, COMMENT_2);
}
