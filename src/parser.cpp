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

void parse(Code &code) {
	printverbose("Parsing " + code.getfile());

	vector<string> lines = code.getlines();

	// Current namespace, might be empty
	string currentns = "";

	// Current method name, if no method, should be "main"
	Method *currentmethod;
	Method main(ENTRY_POINT);
	methodMap.push_back(&main);
	currentmethod = &main;

	int rl = 0;
	for (unsigned int i = 0; i < lines.size(); i++) {
		string line = lines[i];

		line = trim(line);

		if (line.length() == 0) {
			continue;
		}

		rl++;

		bool end = startswith(line, KW_END);

		if (end) {
			line = line.substr(strlen(KW_END), line.size());
		}

		if (is_label(line)) {
			// Get label from line, remove KW_GOTO_LABEL in beginning
			string label = line.substr(1, line.length());

			printverbose("Adding label " + color(VERBOSE_HL) + "\"" + label + "\"" + color(VERBOSE) + " on line #" + to_string(rl));

			// Add pair to goto_labels list

			currentmethod->labels.push_back(make_pair(label, currentmethod->lines.size()));
			continue;
		}

		unsigned int firstsep = line.find_first_of(KEYWORD_SEPARATOR);
		string keyword = firstsep == string::npos ? line : line.substr(0, firstsep);
		line = line.substr(firstsep + 1, line.length());

		printverbose("Keyword " + color(VERBOSE_HL) + keyword + color(VERBOSE) + ", line #" + to_string(rl) + color(VERBOSE_HL) + " \"" + line + "\"");

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

				currentmethod->chunk.end = rl + 1;

				currentmethod = &main;
			} else {
				currentmethod = new Method(currentns, line);
				currentmethod->chunk.start = rl + 1;
				printverbose("Method " + color(VERBOSE_HL) + currentmethod->getdisplayname() + color(VERBOSE) + " initialized");
			}
		} else {
			currentmethod->lines.push_back(lines[i]);
		}
	}

	printverbose(color(COLOR_GREEN) + "Parsing complete");

	invoke();
}

void invoke() {
	invoke(ENTRY_POINT);
}

void invoke(string s) {
	Method* method = NULL;

	for (unsigned int i = 0; i < methodMap.size(); i++) {
		Method *m = methodMap.at(i);

		if (m->getdisplayname() == s) {
			method = m;
			break;
		}
	}

	if (method == NULL) {
		printerror("Could not find method " + color(COLOR_LIGHT_RED) + s);
		return;
	}

	invoke(method);
}

void invoke(Method* method) {
	printverbose("Invoking " + color(VERBOSE_HL) + method->getdisplayname() + color(VERBOSE) + " on line " + color(VERBOSE_HL) + "#" + to_string(method->chunk.start));

	vector<string> lines = method->getlines();

	for (unsigned int i = 0; i < lines.size(); i++) {
		defvar* var = NULL;
		execline(method, &i, 0, var);
	}
}

ReturnType execline(Method* method, unsigned int* i, int indent, defvar* var) {
	string line = trim(method->getlines()[*i]);
	string untrimmed = method->getlines()[*i];

	if (is_comment(line)) {
		printverbose(color(COMMENT) + line, false);
		return ReturnType::NONE;
	}

	unsigned int firstsep = line.find_first_of(KEYWORD_SEPARATOR);
	string keyword = firstsep == string::npos ? line : line.substr(0, firstsep);
	line = line.substr(firstsep + 1, line.length());

	if (line == keyword) {
		line = "";
	}

	printverbose("Executing keyword " + color(VERBOSE_HL) + keyword + color(VERBOSE) + ", line #" + to_string(*i) + " " + color(VERBOSE_HL) + "\"" + line + "\"");

	if (keyword == get_kw(KW_CALL_METHOD)) {
		invoke(line);
	} else if (keyword == get_kw(KW_PRINT)) {
		cout << parse_set_statement(line) << endl;
	} else if (keyword == get_kw(KW_PRINT_ERR)) {
		cerr << color(ERROR) << parse_set_statement(line) << reset() << endl;
	} else if (keyword == get_kw(KW_GOTO)) {
		goto_labels goto_lines = method->labels;
		for (unsigned int s = 0; s < goto_lines.size(); s++) {
			goto_pair pair = goto_lines[s];

			if (pair.first == line) {
				printverbose(color(COLOR_MAGENTA) + "Jumping back to line " + color(VERBOSE_HL) + "#" + to_string(pair.second));
				*i = pair.second - 1;
				break;
			}
		}
	} else if (keyword == get_kw(KW_SET_VAR)) {
		// get type
		int f = line.find_first_of(" ");
		string name = line.substr(0, f);

		// get everything else in line that the variable should be set to
		string statement = line.substr(f + 1);

		setvar(name, statement);
	} else if (keyword == get_kw(KW_IF)) {
		parseif(method, untrimmed, i, indent);
	} else if (keyword == get_kw(KW_WHILE)) {
		parsewhile(method, untrimmed, i, indent);
	} else if (keyword == get_kw(KW_HALT)) {
		exit(get_exit_code(line));
	} else if (keyword == get_kw(KW_BREAK)) {
		return ReturnType::BREAK;
	} else if (keyword == get_kw(KW_RETURN)) {
		if (line.length() > 0) {
			string set = parse_set_statement(line);
			defvar v = setvar("temp", line);
			var = &v;
		} else {
			var = NULL;
		}
		return ReturnType::RETURN;
	} else if (keyword == get_kw(KW_CONTINUE)) {
		return ReturnType::CONTINUE;
	} else {
		printerror("Unknown instruction " + color(ERROR_HL) + keyword + " (" + line + ")" + color(ERROR) + " on line #" + to_string(*i));
	}

	return ReturnType::NONE;
}


void parsewhile(Method* method, string line, unsigned int* i, int indent) {
	printverbose("Checking " + color(VERBOSE_HL) + "while" + color(VERBOSE) + ", condition " + color(VERBOSE_HL) + line);

	unsigned int end = *i;

	vector<While> whiles;

	While *current = NULL;

	int totalend;

	while (end < method->getlines().size()) {
		string temp = method->getlines()[end];

		int s = temp.find_first_not_of('\t');

		if (s == indent && startswith(trim(temp), get_kw(KW_WHILE))) {
			if (current != NULL) {
				current->end = end - 1;
				whiles.push_back(*current);
			}

			current = new While;
			current->start = end;
			*i = end;
		} else if (s == indent && startswith(trim(temp), get_kw(KW_WHILE_END))) {
			if (current != NULL) {
				line = method->getlines()[current->start];
				string cond = line.substr(line.find_first_of(" ") + 1);

				current->end = end - 1;
				whiles.push_back(*current);
				*i = end;
				totalend = end;
				break;
			}
		}

		end++;
	}

	for (While conds : whiles) {
		string line = trim(method->getlines()[conds.start]);

		string cond = line.substr(line.find_first_of(" ") + 1);

		if (check_cond(cond)) {
			*i = conds.start + 1;
			exec:
			ReturnType type = execrange(method, i, conds.end, indent + 1);
			if (type == ReturnType::BREAK) {
				continue;
			}
			if (check_cond(cond) || type == ReturnType::CONTINUE) {
				*i = conds.start + 1;
				goto exec;
			}
			break;
		}
	}
	*i = totalend;
}

void parseif(Method* method, string line, unsigned int* i, int indent) {
	printverbose("Checking " + color(VERBOSE_HL) + "if" + color(VERBOSE) + ", condition " + color(VERBOSE_HL) + line);

	unsigned int end = *i;

	vector<If> ifs;

	If *current = NULL;

	int totalend;

	while (end < method->getlines().size()) {
		string temp = method->getlines()[end];

		int s = temp.find_first_not_of('\t');

		if (s == indent && (startswith(trim(temp), get_kw(KW_IF)) || startswith(trim(temp), get_kw(KW_ELSEIF)) || startswith(trim(temp), get_kw(KW_ELSE)))) {
			if (current != NULL) {
				current->end = end - 1;
				ifs.push_back(*current);
			}

			current = new If;
			current->start = end;
			*i = end;
		} else if (s == indent && startswith(trim(temp), get_kw(KW_ENDIF))) {
			if (current != NULL) {
				current->end = end - 1;
				ifs.push_back(*current);
				*i = end;
				totalend = end;
				break;
			}
		}

		end++;
	}

	for (If conds : ifs) {
		string line = trim(method->getlines()[conds.start]);

		// Is else, we have passed by everything else
		if (line == get_kw(KW_ELSE)) {
			*i = conds.start + 1;
			execrange(method, i, conds.end, indent + 1);
			break;
		}

		string cond = line.substr(line.find_first_of(" ") + 1);

		if (check_cond(cond)) {
			*i = conds.start + 1;
			execrange(method, i, conds.end, indent + 1);
			break;
		}
	}
	*i = totalend;
}

ReturnType execrange(Method* method, unsigned int* i, unsigned int to, int indent) {
	for (unsigned int from = *i; from <= to; from++) {
		defvar* var = NULL;
		ReturnType type = execline(method, &from, indent, var);

		if (type != ReturnType::NONE) {
			return type;
		}
	}

	return ReturnType::NONE;
}

bool check_cond(string line) {
	if (line == get_kw(KW_ELSE)) {
		return true;
	}

	printverbose("Checking condition " + color(VERBOSE_HL) + line);
	if (startswith(line, get_kw(KW_VAR_SIGN_KEY, KW_VAR_SIGN))) {
		line = line.substr(1);

		for (unsigned int i = 0; i < stackMap.size(); i++) {
			defvar v = stackMap[i];
			if (v.name == line) {
				return v.var == get_kw(KW_TRUE);
			}
		}
	}

	printwarning("No condition detected, returning default DEFAULT_COND");
	return DEFAULT_COND;
}

defvar* getvar(string name) {

	for (unsigned int i = 0; i < stackMap.size(); i++) {
		defvar v = stackMap[i];
		if (v.name == name) {
			return &stackMap[i];
		}
	}

	return NULL;
}

string parse_set_statement(string s) {
	printverbose("Checking variable set statement " + color(VERBOSE_HL) + s);

	bool opposite = s[0] == '!';
	bool var = s[opposite ? 1 : 0] == '$';

	bool isstring = s[0] == '\"' && s[s.length() - 1] == '\"';

	// Single variable
	if (!isstring && var && s.find(" ") == string::npos) {
		string name = s.substr(opposite ? 2 : 1);

		defvar v = *getvar(name);
		if (v.name == name) {
			if (opposite) {
				bool yes = v.var == "true";

				s = yes ? "false" : "true";
			} else {
				s = v.var;
			}
		}

	} else {
		if (isstring) {
			s = s.substr(1, s.length() - 2);
		} else {
			s = replaceAll(s, " ", "");
		}
		s = parsevars(s);
	}

	return s;
}

defvar setvar(string name, string statement) {
	printverbose("Setting variable " + color(VERBOSE_HL) + name + color(VERBOSE) + " to " + color(VERBOSE_HL) + "\"" + statement + "\"");

	int index = -1;
	for (unsigned int k = 0; k < stackMap.size(); k++) {
		defvar v = stackMap[k];
		if (v.name == name) {
			index = k;
			break;
		}
	}

	defvar var;
	var.name = name;
	var.var = parse_set_statement(statement);

	if (index != -1) {
		printverbose("Updated " + color(VERBOSE_HL) + name + color(VERBOSE) + " on stack with value " + var.var);
		stackMap.at(index) = var;
	} else {
		printverbose("Added " + color(VERBOSE_HL) + name + color(VERBOSE) + " to stack with value " + var.var);
		stackMap.push_back(var);
	}

	return var;
}

inline bool is_label(string s) {
	return startswith(s, get_kw(KW_GOTO_LABEL_KEY, KW_GOTO_LABEL));
}

inline bool is_comment(string s) {
	return startswith(s, COMMENT_1) || startswith(s, COMMENT_2);
}
