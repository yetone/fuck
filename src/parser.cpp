#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <map>
#include <stdlib.h>
#include <typeinfo>

#include "headers/parser.h"
#include "headers/keywords.h"
#include "headers/code.h"
#include "headers/utils.h"
#include "headers/var.h"
#include "headers/colors.h"

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

		if (is_comment(line) || line.length() == 0) {
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

		printverbose("Keyword " + color(VERBOSE_HL) + keyword + color(VERBOSE) + ", line #" + to_string(i) + color(VERBOSE_HL) + " \"" + line + "\"");

		if (keyword == get_kw(KW_NAMESPACE)) {
			if (end) {
				currentns = "";
			} else {
				currentns = line;
			}
		} else if (keyword == get_kw(KW_METHOD)) {
			if (end) {
				printverbose("Method " + color(VERBOSE_HL) + currentmethod->getdisplayname() + color(VERBOSE) + " finished");
				methodMap.push_back(currentmethod);

				currentmethod = &main;
			} else {
				currentmethod = new Method(currentns, line);
				printverbose("Method " + color(VERBOSE_HL) + currentmethod->getdisplayname() + color(VERBOSE) + " initialized");
			}
		} else {
			currentmethod->lines.push_back(keyword + " " + line);
		}
	}

	printverbose("Parsing complete");

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
	printverbose("Invoking " + color(VERBOSE_HL) + method->getdisplayname());

	vector<string> lines = method->getlines();

	for (unsigned int i = 0; i < lines.size(); i++) {
		string line = lines[i];

		unsigned int firstsep = line.find_first_of(KEYWORD_SEPARATOR);
		string keyword = firstsep == string::npos ? line : line.substr(0, firstsep);
		line = line.substr(firstsep + 1, line.length());

		printverbose("Executing keyword " + color(VERBOSE_HL) + keyword + color(VERBOSE) + ", Line " + color(VERBOSE_HL) + line);

		if (keyword == get_kw(KW_CALL_METHOD)) {
			invoke(line);
		} else if (keyword == get_kw(KW_PRINT)) {
			cout << parsevars(line) << endl;
		} else if (keyword == get_kw(KW_PRINT_ERR)) {
			err(line);
		} else if (keyword == get_kw(KW_GOTO)) {
			goto_labels goto_lines = method->goto_labels;
			for (unsigned int s = 0; s < goto_lines.size(); s++) {
				goto_pair pair = goto_lines[s];

				if (pair.first == line) {
					i = pair.second;
					continue;
				}
			}
		} else if (keyword == get_kw(KW_SET_VAR)) {
			// get type
			int f = line.find_first_of(" ");
			string name = line.substr(0, f);

			// get everything else in line that the variable should be set to
			int f2 = line.find(" ", f + 1);
			string value = line.substr(f + 1, f2 - f - 1);

			printverbose("Setting variable " + color(VERBOSE_HL) + name + color(VERBOSE) + " to " + color(VERBOSE_HL) + "\"" + value + "\"");

			defvar var;
			var.name = name;
			var.var = value;
			stackMap.push_back(var);
		}
	}
}

inline bool is_label(string s) {
	return startswith(s, get_kw(KW_GOTO_LABEL_KEY, KW_GOTO_LABEL));
}

inline bool is_comment(string s) {
	return startswith(s, COMMENT_1) || startswith(s, COMMENT_2);
}
