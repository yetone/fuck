#include "headers/engine.h"

#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <map>
#include <stdlib.h>
#include <typeinfo>
#include <utility>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include "headers/keywords.h"
#include "headers/code.h"
#include "headers/utils.h"
#include "headers/var.h"
#include "headers/colors.h"
#include "headers/mathexp.h"
#include "headers/parser.h"

using namespace std;

extern methodmap methodMap;
extern stackmap stackMap;

void parse(Code &code) {
	printverbose(L"Parsing " + stow(code.getfile()));

	vector<wstring> lines = code.getlines();

	// Current namespace, might be empty
	wstring currentns = L"";

	// Current method name, if no method, should be L"main"
	Method *currentmethod;
	Method main(ENTRY_POINT);
	methodMap.push_back(&main);
	currentmethod = &main;

	int rl = 0;
	for (unsigned int i = 0; i < lines.size(); i++) {
		wstring line = lines[i];

		line = trim(line);

		if (line.length() == 0) {
			continue;
		}

		rl++;

		bool end = line == get_kw(KW_END) || line.substr(0, line.find_first_of(' ')) == get_kw(KW_END);

		if (end) {
			if (line.length() > strlen(KW_END)) {
				line = line.substr(strlen(KW_END) + 1, line.size());
			} else {
				line = L"";
			}
		}

		unsigned int firstsep = line.find_first_of(KEYWORD_SEPARATOR);
		wstring keyword = firstsep == wstring::npos ? line : line.substr(0, firstsep);
		line = line.substr(firstsep + 1, line.length());

		printverbose(L"Keyword " + color(VERBOSE_HL) + keyword + color(VERBOSE) + L", line #" + to_wstring(rl) + color(VERBOSE_HL) + L" \"" + line + L"\"");

		if (keyword == get_kw(KW_NAMESPACE)) {
			if (end) {
				currentns = L"";
			} else {
				currentns = line;
			}
		} else if (keyword == get_kw(KW_METHOD)) {
			if (end) {
				printverbose(L"Method " + color(VERBOSE_HL) + currentmethod->getdisplayname() + color(VERBOSE) + L" finished");
				methodMap.push_back(currentmethod);

				currentmethod->chunk.end = rl + 1;

				currentmethod = &main;
			} else {
				currentmethod = new Method(currentns, line);
				currentmethod->chunk.start = rl + 1;
				printverbose(L"Method " + color(VERBOSE_HL) + currentmethod->getdisplayname() + color(VERBOSE) + L" initialized");
			}
		} else {
			currentmethod->lines.push_back(lines[i]);
		}
	}

	printverbose(color(COLOR_GREEN) + L"Parsing complete");

	invoke();
}

void invoke() {
	invoke(L"main");
}

variable* invoke(wstring s) {
	Method* method = nullptr;

	for (unsigned int i = 0; i < methodMap.size(); i++) {
		Method *m = methodMap.at(i);

		if (m->getdisplayname() == s) {
			method = m;
			break;
		}
	}

	if (method == nullptr) {
		printerror(L"Could not find method " + color(COLOR_LIGHT_RED) + s);
		return nullptr;
	}

	return invoke(method);
}

variable* invoke(Method* method) {
	printverbose(L"Invoking " + color(VERBOSE_HL) + method->getdisplayname() + color(VERBOSE) + L" on line " + color(VERBOSE_HL) + L"#" + to_wstring(method->chunk.start));

	vector<wstring> lines = method->getlines();

	for (unsigned int i = 0; i < lines.size(); i++) {
		variable* var = nullptr;

		ReturnType type = execline(method, &i, 0, var);

		if (type == ReturnType::RETURN && var != nullptr) {
			return var;
		}
	}

	return nullptr;
}

ReturnType execline(Method* method, unsigned int* i, int indent, variable*& var) {
	wstring untrimmed = method->getlines()[*i];

	wstring line = trim(untrimmed);

	if (is_comment(line)) {
		printverbose(line, verbose_mode::COMMENT);
		return ReturnType::NONE;
	}

	unsigned int firstsep = line.find_first_of(KEYWORD_SEPARATOR);
	wstring keyword = firstsep == wstring::npos ? line : line.substr(0, firstsep);
	line = line.substr(firstsep + 1, line.length());

	if (line == keyword) {
		line = L"";
	}

	printverbose(L"Executing keyword " + color(VERBOSE_HL) + keyword + color(VERBOSE) + L", line #" + to_wstring(*i) + L" " + color(VERBOSE_HL) + L"\"" + line + L"\"");

	if (keyword == get_kw(KW_CALL_METHOD)) {
		variable* returned = invoke(line);

		if (returned != nullptr) {
			printverbose(color(COLOR_MAGENTA) + L"Returned value " + returned->get());
		}
	} else if (keyword == get_kw(KW_PRINT)) {
		wcout << parse_set_statement(line) << endl;
	} else if (keyword == get_kw(KW_PRINT_ERR)) {
		wcerr << color(ERROR_COLOR) << parse_set_statement(line) << reset() << endl;
	} else if (keyword == get_kw(KW_GOTO)) {
		vector<wstring> lines = method->getlines();

		for (unsigned int l = 0; l < lines.size(); l++) {
			if (lines[l] == get_kw(KW_LABEL) + L" L" + line)  {
				*i = l + 1;
				return ReturnType::NONE;
			}
		}
	} else if (keyword == get_kw(KW_IF)) {
		ReturnType type = parseif(method, untrimmed, i, indent, var);
		return type;
	} else if (keyword == get_kw(KW_WHILE)) {
		ReturnType type = parsewhile(method, untrimmed, i, indent, var);
		return type;
	} else if (keyword == get_kw(KW_FOR)) {
		ReturnType type = parsefor(method, untrimmed, i, indent, var);
		return type;
	} else if (keyword == get_kw(KW_HALT)) {
		exit(get_exit_code(line));
	} else if (keyword == get_kw(KW_BREAK)) {
		return ReturnType::BREAK;
	} else if (keyword == get_kw(KW_RETURN)) {
		if (line.length() > 0) {
			printverbose(L"Returning " + color(VERBOSE_HL) + line);
			wstring set = parse_set_statement(line);
			variable* v = setvar(L"temp", line);
			var = v;
		} else {
			var = nullptr;
			printverbose(L"Returning...");
		}
		return ReturnType::RETURN;
	} else if (keyword == get_kw(KW_CONTINUE)) {
		return ReturnType::CONTINUE;
	} else if (keyword == get_kw(KW_UNSET_VAR)) {
		unset(trim(line));
	} else if (keyword == get_kw(KW_SET_VAR) || (startswith(keyword, get_kw(KW_VAR_SIGN_KEY, KW_VAR_SIGN)) && startswith(line, get_kw(KW_PUSH_VAR_SIMPLE_KEY, KW_PUSH_VAR_SIMPLE)))) {
		// get type
		int f = line.find_first_of(L" ");
		wstring name;
		wstring statement;
		type t = type::DEFAULT;

		if (startswith(keyword, get_kw(KW_VAR_SIGN_KEY, KW_VAR_SIGN))) {
			name = keyword.substr(1);
			statement = trim(line.substr(2));
		} else {
			name = line.substr(0, f);
			statement = line.substr(f + 1);

			if (name == get_kw(KW_SET_FRONT) || name == get_kw(KW_SET_BACK)) {
				name = statement.substr(0, statement.find_first_of(L" "));
				statement = statement.substr(statement.find_first_of(L" ") + 1);

				if (name == get_kw(KW_SET_FRONT)) {
					// TODO variable should be pushed to bottom of stack (first
				}
			}
		}

		unsigned int firstb = name.find(L"(");
		unsigned int lastb = name.find(L")");

		if (firstb != string::npos && lastb != string::npos) {
			wstring key = name.substr(firstb + 1, lastb - firstb - 1);
			name = name.substr(0, firstb);

			//wcout << L"Name: " <<name << endl << L"Key: " << key << endl << L"Value: " << statement << endl;

			statement = key + L" > " + statement;

			t = type::ARRAY;
		}

		setvar(name, statement, t);
	} else if (keyword == get_kw(KW_LABEL)) {
		printverbose(L"Ignoring label at line #" + to_wstring(*i));
	} else if (keyword == get_kw(KW_READ)) {
		wstring s;

		getline(wcin, s);

		if (line.length() > 0) {
			vector<wstring> vars = split(line, ' ');

			for (wstring var : vars) {
				setvar(var, L"\"" + s + L"\"");
			}
		}
	} else if (keyword == get_kw(KW_SLEEP)) {
		#ifdef _WIN32
		Sleep(wtoi(line));
		#else
		usleep(wtoi(line) * 1000);
		#endif
	} else if (keyword == get_kw(KW_INCR) || keyword == get_kw(KW_DECR)) {
		int f = line.find_first_of(L" ");

		wstring name = line.substr(0, f);
		wstring statement = line.substr(f + 1);

		bool incr = keyword == get_kw(KW_INCR);

		if (statement == name) {
			statement = name + (incr ? L" + " : L" - ") + L"1";
		} else {
			statement = name + (incr ? L" + " : L" - ") + statement;
		}

		setvar(name, statement);
	} else if (keyword == get_kw(KW_ARRAY)) {
		int f = line.find_first_of(L" ");

		wstring name = line.substr(0, f);
		wstring statementstr = line.substr(f + 2, line.length() - f - 3);

		vector<wstring> statements = split(statementstr, ',');

		setvar(name, statements, type::ARRAY);
	} else {
		printerror(L"Unknown instruction " + color(ERROR_HL) + keyword + L" (" + line + L")" + color(ERROR_COLOR) + L" on line #" + to_wstring(*i));
	}

	return ReturnType::NONE;
}

ReturnType parsefor(Method* method, wstring line, unsigned int* i, int indent, variable*& var) {
	printverbose(L"Checking " + color(VERBOSE_HL) + L"for" + color(VERBOSE) + L", condition " + color(VERBOSE_HL) + line);

	int totalend;

	vector<wstring> keywords;
	keywords.push_back(get_kw(KW_FOR));

	vector<Chunk> chunks = parse_chunks(method, i, indent, &totalend, keywords);

	for (Chunk chunk : chunks) {
		wstring line = trim(method->getlines()[chunk.start]);

		wstring cond = line.substr(line.find_first_of(L" L") + 1);
		vector<wstring> spl = split(cond, ';');

		wstring first = cond.substr(0, cond.find_first_of(L";"));
		if (first[0] == get_kw(KW_VAR_SIGN_KEY, KW_VAR_SIGN)[0]) {
			first = first.substr(1);
		}

		variable* from;
		int to;
		wstring dos;

		for (unsigned int in = 1; in < spl.size(); in++) {
			wstring s = trim(spl[in]);
			wstring word = s.substr(0, s.find_first_of(' '));
			wstring w = s.substr(s.find_first_of(' ') + 1);

			if (startswith(s, get_kw(KW_FOR_FROM))) {
				from = setvar(first, w);
			} else if (startswith(s, get_kw(KW_FOR_TO))) {
				to = wtoi(w);
			} else if (startswith(s, get_kw(KW_FOR_DO))) {
				dos = w;
			} else {
				printwarning(L"Unknown " + s);
			}
		}

		int f = wtoi(from->get());

		if (f != to) {
			*i = chunk.start + 1;
			exec:
			ReturnType type = execrange(method, i, chunk.end, indent + 1, var);
			if (type == ReturnType::BREAK) {
				continue;
			} else if (f != to || type == ReturnType::CONTINUE) {
				*i = chunk.start + 1;

				variable* temp = setvar(from->name, dos);
				f = wtoi(temp->get());
				goto exec;
			} else if (type == ReturnType::RETURN && var != nullptr) {
				return type;
			}
			break;
		}
	}
	*i = totalend;

	return ReturnType::NONE;
}

ReturnType parsewhile(Method* method, wstring line, unsigned int* i, int indent, variable*& var) {
	printverbose(L"Checking " + color(VERBOSE_HL) + L"while" + color(VERBOSE) + L", condition " + color(VERBOSE_HL) + line);

	int totalend;

	vector<wstring> keywords;
	keywords.push_back(get_kw(KW_WHILE));

	vector<Chunk> chunks = parse_chunks(method, i, indent, &totalend, keywords);

	for (Chunk chunk : chunks) {
		wstring line = trim(method->getlines()[chunk.start]);

		wstring cond = line.substr(line.find_first_of(L" L") + 1);

		if (check_cond(cond)) {
			*i = chunk.start + 1;
			exec: ReturnType type = execrange(method, i, chunk.end, indent + 1, var);

			if (type == ReturnType::BREAK) {
				*i = chunk.end;
				continue;
			} else if (type == ReturnType::RETURN && var != nullptr) {
				return type;
			} else if (check_cond(cond) || type == ReturnType::CONTINUE) {
				*i = chunk.start + 1;
				goto exec;
			}
			break;
		}
	}
	*i = totalend;

	return ReturnType::NONE;
}

ReturnType parseif(Method* method, wstring line, unsigned int* i, int indent, variable*& var) {
	printverbose(L"Checking " + color(VERBOSE_HL) + L"if" + color(VERBOSE) + L", condition " + color(VERBOSE_HL) + line);

	int totalend;

	vector<wstring> keywords;
	keywords.push_back(get_kw(KW_IF));
	keywords.push_back(get_kw(KW_ELSEIF));
	keywords.push_back(get_kw(KW_ELSE));

	vector<Chunk> chunks = parse_chunks(method, i, indent, &totalend, keywords);

	for (Chunk conds : chunks) {
		wstring line = trim(method->getlines()[conds.start]);

		// Is else, we have passed by everything else
		if (line == get_kw(KW_ELSE)) {
			*i = conds.start + 1;
			ReturnType type = execrange(method, i, conds.end, indent + 1, var);
			if (type == ReturnType::RETURN) {
				return type;
			}
			break;
		}

		wstring cond = line.substr(line.find_first_of(L" L") + 1);

		if (check_cond(cond)) {
			*i = conds.start + 1;
			ReturnType type = execrange(method, i, conds.end, indent + 1, var);
			if (type != ReturnType::NONE) {
				return type;
			}
			break;
		}
	}
	*i = totalend;

	return ReturnType::NONE;
}

ReturnType execrange(Method* method, unsigned int* i, unsigned int to, int indent, variable*& var) {
	for (unsigned int from = *i; from <= to; from++) {
		ReturnType type = execline(method, &from, indent, var);

		if (type != ReturnType::NONE) {
			return type;
		}
	}

	return ReturnType::NONE;
}

bool check_cond(wstring line) {
	if (line == get_kw(KW_ELSE)) {
		return true;
	}

	bool var = startswith(line.substr(0), get_kw(KW_VAR_SIGN_KEY, KW_VAR_SIGN));
	if (var) {
		line = L"(" + line + L")";
	}

	vector<pair<Conds, bool>> conds;

	int pos = 0;
	while (true) {
		int first = line.find('(', pos);
		int second = line.find(')', pos);

		int beginnext = line.find('(', pos + 1);

		Conds cond = Conds::AND;

		if (beginnext != (signed int) wstring::npos) {
			wstring keywords = trim(line.substr(second + 1, beginnext - second - 1));

			if (contains(keywords, get_kw(KW_AND)) || contains(keywords, get_kw(OP_AND))) {
				cond = Conds::AND;
			} else if (contains(keywords, get_kw(KW_XOR)) || contains(keywords, get_kw(OP_XOR))) {
				cond = Conds::XOR;
			} else if (contains(keywords, get_kw(KW_OR)) || contains(keywords, get_kw(OP_OR))) {
				cond = Conds::OR;
			}
		}

		if (first == (signed int) wstring::npos || second == (signed int) wstring::npos) {
			break;
		}

		wstring s = line.substr(first + 1, second - 1);
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

bool check_cond_compare(wstring cond) {
	wstring splitat;
	Relational ret;

	if (contains(cond, get_kw(OP_EQUALS))) {
		splitat = get_kw(OP_EQUALS);
		ret = Relational::EQUALS;
	} else if (contains(cond, get_kw(OP_NOT_EQ))) {
		splitat = get_kw(OP_NOT_EQ);
		ret = Relational::NOT_EQUAL;
	} else if (contains(cond, get_kw(OP_LESS_THAN))) {
		splitat = get_kw(OP_LESS_THAN);
		ret = Relational::LESS_THAN;
	} else if (contains(cond, get_kw(OP_MORE_THAN))) {
		splitat = get_kw(OP_MORE_THAN);
		ret = Relational::MORE_THAN;
	} else if (contains(cond, get_kw(OP_LESS_OR_EQUALS))) {
		splitat = get_kw(OP_LESS_OR_EQUALS);
		ret = Relational::LESS_OR_EQUALS;
	} else if (contains(cond, get_kw(OP_MORE_OR_EQUALS))) {
		splitat = get_kw(OP_MORE_OR_EQUALS);
		ret = Relational::MORE_OR_EQUALS;
	} else {
		ret = Relational::SINGLE;
	}

	int index = cond.find(splitat);

	wstring stat1 = parse_set_statement(trim(cond.substr(0, index)));
	wstring stat2 = parse_set_statement(trim(cond.substr(index + splitat.length())));

	if (stat1.length() == 0) {
		stat1 = stat2;
		stat2 = L"";
	}

	return check_cond_compare(stat1, stat2, ret);
}

bool check_cond_compare(const wstring& var1, const wstring& var2, Relational ret) {
	if (ret == Relational::SINGLE) {
		return var1 == get_kw(KW_TRUE);
	} else if (ret == Relational::EQUALS) {
		return var1 == var2;
	} else if (ret == Relational::NOT_EQUAL) {
		return var1 != var2;
	}

	double i1 = wtod(var1);
	double i2 = wtod(var2);

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

variable* getvar(wstring name) {
	if (name[0] == get_kw(KW_VAR_SIGN_KEY, KW_VAR_SIGN)[0]) {
		name = name.substr(1);
	}

	for (unsigned int i = 0; i < stackMap.size(); i++) {
		variable* v = stackMap[i];
		if (v->name == name) {
			return stackMap[i];
		}
	}

	return nullptr;
}

wstring parse_set_statement(wstring s) {
	printverbose(L"Checking variable set statement " + color(VERBOSE_HL) + s);

	if (s.length() == 0) {
		return s;
	}

	s = trim(s);

	bool opposite = s[0] == L'!';
	bool var = s[opposite ? 1 : 0] == get_kw(KW_VAR_SIGN_KEY, KW_VAR_SIGN);

	bool iswstring = s[0] == L'\"' && s[s.length() - 1] == L'\"';

	// Single variable
	if (!iswstring && var && s.find(L" ") == wstring::npos) {
		wstring name = s.substr(opposite ? 2 : 1);

		variable* v = getvar(name);
		if (v != nullptr && v->name == name) {
			if (opposite) {
				bool yes = v->get() == get_kw(KW_TRUE);

				s = yes ? get_kw(KW_FALSE) : get_kw(KW_TRUE);
			} else {
				s = v->get();
			}
		}

	} else if (s != get_kw(KW_TRUE) && s != get_kw(KW_FALSE)) {
		s = parsevars(s);

		if (iswstring) {
			s = s.substr(1, s.length() - 2);
		} else {
			s = replaceAll(s, L" ", L"");
			double val = eval(wtos(s));

			s = to_wstring(val);

			unsigned int f = s.find_last_of('.');

			if (f != wstring::npos && s.substr(f + 1).find_first_not_of('0') == wstring::npos) {
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

variable* setvar(wstring name, wstring statement, type t) {
	vector<wstring> statements;
	statements.push_back(statement);
	return setvar(name, statements, t);
}

variable* setvar(wstring name, vector<wstring> statements, type t) {
	//printverbose(L"Setting variable " + color(VERBOSE_HL) + name + color(VERBOSE) + L" to " + color(VERBOSE_HL) + L"\"" + statement + L"\"", verbose_mode::ADDITION);

	if (name[0] == get_kw(KW_VAR_SIGN_KEY, KW_VAR_SIGN)[0]) {
		name = name.substr(1);
	}

	variable* var = nullptr;

	int index = -1;
	for (unsigned int k = 0; k < stackMap.size(); k++) {
		variable* v = stackMap[k];
		if (v->name == name) {
			var = v;
			index = k;
			break;
		}
	}

	if (t == type::DEFAULT) {
		if (var == nullptr) {
			var = new str;
		}

		var->set(parse_set_statement(statements[0]));
	} else if (t == type::ARRAY) {
		arrays* arr;

		if (var == nullptr) {
			arr = new arrays;
		} else {
			arr = (arrays*) var;
		}

		for (unsigned int i = 0; i < statements.size(); i++) {
			vector<wstring> kw = split(statements[i], L'>');

			wstring key;
			wstring statement;

			if (kw.size() == 1) {
				key = itow(arr->var.size());
				statement = kw[0];
			} else {
				key = kw[0];
				statement = kw[1];
			}

			printverbose(L"Setting " + key + L" to " + statement, verbose_mode::ADDITION);

			arr->var[parse_set_statement(trim(key))] = parse_set_statement(trim(statement));
		}

		var = arr;
	}

	var->name = name;

	if (index != -1) {
		printverbose(L"Updated " + color(VERBOSE_HL) + name + color(VERBOSE) + L" on stack with value " + var->get(), verbose_mode::ADDITION);
		stackMap.at(index) = var;
	} else {
		printverbose(L"Added " + color(VERBOSE_HL) + name + color(VERBOSE) + L" to stack with value " + var->get(), verbose_mode::ADDITION);

		stackMap.insert(stackMap.begin(), var);
	}

	return var;
}

void unset(wstring name) {
	unset(getvar(name));
}

void unset(variable* var) {
	if (var != nullptr) {
		for (unsigned int i = 0; i < stackMap.size(); i++) {
			variable* var2 = stackMap[i];

			if (var2->name == var->name) {
				stackMap.erase(stackMap.begin() + i);
				break;
			}
		}

		printverbose(L"Deleting " + var->name, verbose_mode::DELETION);

		delete var;
	}
}

inline bool is_comment(wstring s) {
	return startswith(s, COMMENT_1) || startswith(s, COMMENT_2);
}
