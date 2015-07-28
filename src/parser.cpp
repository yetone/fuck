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
#include "headers/mathexp.h"

using namespace std;

extern methodmap methodMap;
extern stackmap stackMap;

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

		bool end = line.substr(0, line.find_first_of(' ')) == get_kw(KW_END);

		if (end) {
			if (line.length() > strlen(KW_END)) {
				line = line.substr(strlen(KW_END) + 1, line.size());
			} else {
				line = "";
			}
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

Variable* invoke(string s) {
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
		return NULL;
	}

	return invoke(method);
}

Variable* invoke(Method* method) {
	printverbose("Invoking " + color(VERBOSE_HL) + method->getdisplayname() + color(VERBOSE) + " on line " + color(VERBOSE_HL) + "#" + to_string(method->chunk.start));

	vector<string> lines = method->getlines();

	for (unsigned int i = 0; i < lines.size(); i++) {
		Variable* var = NULL;

		ReturnType type = execline(method, &i, 0, var);

		if (type == ReturnType::RETURN && var != NULL) {
			return var;
		}
	}

	return NULL;
}

ReturnType execline(Method* method, unsigned int* i, int indent, Variable*& var) {
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
		Variable* returned = invoke(line);

		if (returned != NULL) {
			printverbose(color(COLOR_MAGENTA) + "Returned value " + returned->var);
		}
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
	} else if (keyword == get_kw(KW_IF)) {
		// TODO parse return
		parseif(method, untrimmed, i, indent, var);
	} else if (keyword == get_kw(KW_WHILE)) {
		// TODO parse return
		parsewhile(method, untrimmed, i, indent, var);
	} else if (keyword == get_kw(KW_FOR)) {
		// TODO parse return
		parsefor(method, untrimmed, i, indent, var);
	} else if (keyword == get_kw(KW_HALT)) {
		exit(get_exit_code(line));
	} else if (keyword == get_kw(KW_BREAK)) {
		return ReturnType::BREAK;
	} else if (keyword == get_kw(KW_RETURN)) {
		if (line.length() > 0) {
			printverbose("Returning " + color(VERBOSE_HL) + line);
			string set = parse_set_statement(line);
			Variable v = setvar("temp", line);
			var = &v;
		} else {
			var = NULL;
			printverbose("Returning...");
		}
		return ReturnType::RETURN;
	} else if (keyword == get_kw(KW_CONTINUE)) {
		return ReturnType::CONTINUE;
	} else if (keyword == get_kw(KW_PUSH_VAR) || (startswith(keyword, get_kw(KW_VAR_SIGN_KEY, KW_VAR_SIGN)) && startswith(line, get_kw(KW_PUSH_VAR_SIMPLE_KEY, KW_PUSH_VAR_SIMPLE)))) {
		// get type
		int f = line.find_first_of(" ");
		string name;
		string statement;
		StackPos pos = StackPos::END;

		if (startswith(keyword, get_kw(KW_VAR_SIGN_KEY, KW_VAR_SIGN))) {
			name = keyword.substr(1);
			statement = trim(line.substr(2));
		} else {
			name = line.substr(0, f);
			statement = line.substr(f + 1);

			if (name == get_kw(KW_PUSH_FRONT) || name == get_kw(KW_PUSH_BACK)) {
				name = statement.substr(0, statement.find_first_of(" "));
				statement = statement.substr(statement.find_first_of(" ") + 1);

				if (name == get_kw(KW_PUSH_FRONT)) {
					pos = StackPos::FRONT;
				}
			}
		}

		setvar(name, statement, pos);
	} else {
		printerror("Unknown instruction " + color(ERROR_HL) + keyword + " (" + line + ")" + color(ERROR) + " on line #" + to_string(*i));
	}

	return ReturnType::NONE;
}

ReturnType parsefor(Method* method, string line, unsigned int* i, int indent, Variable*& var) {
	printverbose("Checking " + color(VERBOSE_HL) + "for" + color(VERBOSE) + ", condition " + color(VERBOSE_HL) + line);

	unsigned int end = *i;

	vector<For> fors;

	For *current = NULL;

	int totalend;

	while (end < method->getlines().size()) {
		string temp = method->getlines()[end];

		int s = temp.find_first_not_of('\t');

		if (s == indent && startswith(trim(temp), get_kw(KW_FOR))) {
			if (current != NULL) {
				current->end = end - 1;
				fors.push_back(*current);
			}

			current = new For;
			current->start = end;
			*i = end;
		} else if (s == indent && startswith(trim(temp), get_kw(KW_END))) {
			if (current != NULL) {
				line = method->getlines()[current->start];
				string cond = line.substr(line.find_first_of(" ") + 1);

				current->end = end - 1;
				fors.push_back(*current);
				*i = end;
				totalend = end;
				break;
			}
		}

		end++;
	}

	for (For conds : fors) {
		string line = trim(method->getlines()[conds.start]);

		string cond = line.substr(line.find_first_of(" ") + 1);
		vector<string> spl = split(cond, ';');

		string first = cond.substr(0, cond.find_first_of(";"));
		if (first[0] == get_kw(KW_VAR_SIGN_KEY, KW_VAR_SIGN)[0]) {
			first = first.substr(1);
		}

		Variable from;
		int to;
		string dos;

		for (unsigned int in = 1; in < spl.size(); in++) {
			string s = trim(spl[in]);
			string word = s.substr(0, s.find_first_of(' '));
			string w = s.substr(s.find_first_of(' ') + 1);

			if (startswith(s, get_kw(KW_FOR_FROM))) {
				from = setvar(first, w);
			} else if (startswith(s, get_kw(KW_FOR_TO))) {
				to = atoi(w.c_str());
			} else if (startswith(s, get_kw(KW_FOR_DO))) {
				dos = w;
			} else {
				printwarning("Unknown " + s);
			}
		}

		int f = atoi(from.var.c_str());

		if (f != to) {
			*i = conds.start + 1;
			exec:
			ReturnType type = execrange(method, i, conds.end, indent + 1, var);
			if (type == ReturnType::BREAK) {
				continue;
			} else if (f != to || type == ReturnType::CONTINUE) {
				*i = conds.start + 1;

				Variable temp = setvar(from.name, dos);
				f = atoi(temp.var.c_str());
				goto exec;
			} else if (type == ReturnType::RETURN && var != NULL) {
				return type;
			}
			break;
		}
	}
	*i = totalend;

	return ReturnType::NONE;
}

ReturnType parsewhile(Method* method, string line, unsigned int* i, int indent, Variable*& var) {
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
		} else if (s == indent && startswith(trim(temp), get_kw(KW_END))) {
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
			ReturnType type = execrange(method, i, conds.end, indent + 1, var);
			if (type == ReturnType::BREAK) {
				continue;
			} else if (check_cond(cond) || type == ReturnType::CONTINUE) {
				*i = conds.start + 1;
				goto exec;
			} else if (type == ReturnType::RETURN && var != NULL) {
				return type;
			}
			break;
		}
	}
	*i = totalend;

	return ReturnType::NONE;
}

ReturnType parseif(Method* method, string line, unsigned int* i, int indent, Variable*& var) {
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
		} else if (s == indent && startswith(trim(temp), get_kw(KW_END))) {
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
			ReturnType type = execrange(method, i, conds.end, indent + 1, var);
			if (type == ReturnType::RETURN) {
				return type;
			}
			break;
		}

		string cond = line.substr(line.find_first_of(" ") + 1);

		if (check_cond(cond)) {
			*i = conds.start + 1;
			ReturnType type = execrange(method, i, conds.end, indent + 1, var);
			if (type == ReturnType::RETURN) {
				return type;
			}
			break;
		}
	}
	*i = totalend;

	return ReturnType::NONE;
}

ReturnType execrange(Method* method, unsigned int* i, unsigned int to, int indent, Variable*& var) {
	for (unsigned int from = *i; from <= to; from++) {
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

	bool var = startswith(line, get_kw(KW_VAR_SIGN_KEY, KW_VAR_SIGN));
	if (var) {
		line = line.substr(1);

		for (unsigned int i = 0; i < stackMap.size(); i++) {
			Variable v = stackMap[i];
			if (v.name == line) {
				printverbose("Matched single variable");
				return v.var == get_kw(KW_TRUE);
			}
		}
	}

	vector<pair<Conds, bool>> conds;

	int pos = 0;
	while (true) {
		int first = line.find('(', pos);
		int second = line.find(')', pos);

		int beginnext = line.find('(', pos + 1);

		Conds cond = Conds::AND;

		if (beginnext != (signed int) string::npos) {
			string keywords = trim(line.substr(second + 1, beginnext - second - 1));

			if (contains(keywords, get_kw(KW_AND))) {
				cond = Conds::AND;
			} else if (contains(keywords, get_kw(KW_XOR))) {
				cond = Conds::XOR;
			} else if (contains(keywords, get_kw(KW_OR))) {
				cond = Conds::OR;
			}
		}

		if (first == (signed int) string::npos || second == (signed int) string::npos) {
			break;
		}

		string s = line.substr(first + 1, second - 1);
		pos += s.length();

		bool result = check_cond_compare(s);

		conds.push_back(make_pair(cond, result));
	}

	bool istrue = false;

	bool b[conds.size()];
	for (unsigned int i = 0; i < conds.size(); i++) {
		b[i] = false;
	}

	bool exor = false;

	pos = 0;
	for (unsigned int i = 0; i < conds.size(); i++) {
		pair<Conds, bool> p = conds[i];

		if (p.first == Conds::AND) {
			b[pos] = p.second;
			pos++;
		} else if (p.first == Conds::OR) {
			b[pos++] = p.second;
		} else if (p.first == Conds::XOR) {
			b[pos++] = p.second;
			exor = true;
		}
	}

	if (exor) {
		istrue = false;
		bool first = b[0];

		for (int i = 0; i < pos; i++) {
			//cout << "first: " << first << ", i: " << b[i] << endl;
			if (b[i] != first) {
				istrue = true;
				break;
			}
		}
	} else {
		for (bool bs : b) {
			if (bs) {
				istrue = bs;
				break;
			}
		}
	}

	return istrue;
}

bool check_cond_compare(string cond) {
	string splitat;
	Relational ret;

	if (contains(cond, get_kw(KW_EQUALS))) {
		splitat = get_kw(KW_EQUALS);
		ret = Relational::EQUALS;
	} else if (contains(cond, get_kw(KW_NOT_EQ))) {
		splitat = get_kw(KW_NOT_EQ);
		ret = Relational::NOT_EQUAL;
	} else if (contains(cond, get_kw(KW_LESS_THAN))) {
		splitat = get_kw(KW_LESS_THAN);
		ret = Relational::LESS_THAN;
	} else if (contains(cond, get_kw(KW_MORE_THAN))) {
		splitat = get_kw(KW_MORE_THAN);
		ret = Relational::MORE_THAN;
	} else if (contains(cond, get_kw(KW_LESS_OR_EQUALS))) {
		splitat = get_kw(KW_LESS_OR_EQUALS);
		ret = Relational::LESS_OR_EQUALS;
	} else if (contains(cond, get_kw(KW_MORE_OR_EQUALS))) {
		splitat = get_kw(KW_MORE_OR_EQUALS);
		ret = Relational::MORE_OR_EQUALS;
	}

	int index = cond.find(splitat);

	string stat1 = parse_set_statement(trim(cond.substr(0, index)));
	string stat2 = parse_set_statement(trim(cond.substr(index + splitat.length())));

	return check_cond_compare(stat1, stat2, ret);
}

bool check_cond_compare(const string& var1, const string& var2, Relational ret) {
	if (ret == Relational::EQUALS) {
		return var1 == var2;
	} else if (ret == Relational::NOT_EQUAL) {
		return var1 != var2;
	}

	double i1 = atof(var1.c_str());
	double i2 = atof(var2.c_str());

	if (ret == Relational::LESS_THAN) {
		return i1 < i2;
	} else if (ret == Relational::MORE_THAN) {
		return i1 > i2;
	} else if (ret == Relational::LESS_OR_EQUALS) {
		return i1 <= i2;
	} else if (ret == Relational::MORE_OR_EQUALS) {
		return i1 >= i2;
	}

	return DEFAULT_COND;
}

Variable* getvar(string name) {
	if (name[0] == get_kw(KW_VAR_SIGN_KEY, KW_VAR_SIGN)[0]) {
		name = name.substr(1);
	}

	for (unsigned int i = 0; i < stackMap.size(); i++) {
		Variable v = stackMap[i];
		if (v.name == name) {
			return &stackMap[i];
		}
	}

	return NULL;
}

string parse_set_statement(string s) {
	printverbose("Checking variable set statement " + color(VERBOSE_HL) + s);

	if (s.length() == 0) {
		return s;
	}

	s = trim(s);

	bool opposite = s[0] == '!';
	bool var = s[opposite ? 1 : 0] == '$';

	bool isstring = s[0] == '\"' && s[s.length() - 1] == '\"';

	// Single variable
	if (!isstring && var && s.find(" ") == string::npos) {
		string name = s.substr(opposite ? 2 : 1);

		Variable v = *getvar(name);
		if (v.name == name) {
			if (opposite) {
				bool yes = v.var == get_kw(KW_TRUE);

				s = yes ? get_kw(KW_FALSE) : get_kw(KW_TRUE);
			} else {
				s = v.var;
			}
		}

	} else if (s != get_kw(KW_TRUE) && s != get_kw(KW_FALSE)){
		s = parsevars(s);

		if (isstring) {
			s = s.substr(1, s.length() - 2);
		} else {
			s = replaceAll(s, " ", "");
			double val = eval(s);

			s = to_string(val);

			unsigned int f = s.find_last_of('.');

			if (f != string::npos && s.substr(f + 1).find_first_not_of('0') == string::npos) {
				// If absolute value, remove dot and zeros
				s = s.substr(0, f);
			} else {
				// Remove zeros
				s = s.substr(0, s.find_last_not_of('0') + 1);
			}
		}
	}

	return s;
}

Variable setvar(string name, string statement, StackPos pos) {
	printverbose("Setting variable " + color(VERBOSE_HL) + name + color(VERBOSE) + " to " + color(VERBOSE_HL) + "\"" + statement + "\"");

	if (name[0] == get_kw(KW_VAR_SIGN_KEY, KW_VAR_SIGN)[0]) {
		name = name.substr(1);
	}

	int index = -1;
	for (unsigned int k = 0; k < stackMap.size(); k++) {
		Variable v = stackMap[k];
		if (v.name == name) {
			index = k;
			break;
		}
	}

	Variable var;
	var.name = name;
	var.var = parse_set_statement(statement);

	if (index != -1) {
		printverbose("Updated " + color(VERBOSE_HL) + name + color(VERBOSE) + " on stack with value " + var.var);
		stackMap.at(index) = var;
	} else {
		printverbose("Added " + color(VERBOSE_HL) + name + color(VERBOSE) + " to stack with value " + var.var);

		if (pos == StackPos::FRONT) {
			stackMap.insert(stackMap.begin(), var);
		} else if (pos == StackPos::END) {
			stackMap.push_back(var);
		} else {
			printerror("Invalid stack position");
		}
	}

	return var;
}

inline bool is_label(string s) {
	return startswith(s, get_kw(KW_GOTO_LABEL_KEY, KW_GOTO_LABEL));
}

inline bool is_comment(string s) {
	return startswith(s, COMMENT_1) || startswith(s, COMMENT_2);
}
