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

#include "headers/keywords.h"
#include "headers/code.h"
#include "headers/utils.h"
#include "headers/var.h"
#include "headers/colors.h"
#include "headers/mathexp.h"
#include "headers/parser.h"
#include "stl/native.h"

using namespace std;

extern methodmap methodMap;

void invoke() {
	invoke(L"main");
}

variable* invoke(wstring s, bool native) {
	wstring methodname;
	parameters params;

	int sep = s.find(' ');

	if (sep != (signed int) string::npos) {
		methodname = s.substr(0, sep);
		s = s.substr(sep + 1);

		params = split_ignore_strings(s, L",");
	} else {
		methodname = s;
	}

	Method* method = nullptr;

	if (native) {
		method = new NativeMethod(methodname);
	} else {
		for (unsigned int i = 0; i < methodMap.size(); i++) {
			Method *m = methodMap.at(i);

			if (m->getdisplayname() == methodname) {
				method = m;
				break;
			}
		}
	}

	if (method == nullptr) {
		printerror(L"Could not find method " + color(COLOR_LIGHT_RED) + s);
		return nullptr;
	}

	return invoke(method, params);
}

variable* invoke(Method* method, parameters& params) {
	printverbose(L"Invoking " + color(VERBOSE_HL) + method->getdisplayname() + color(VERBOSE) + L" on line " + color(VERBOSE_HL) + L"#" + itow(method->chunk.start));

	linemap lines = method->getlines();

	stackmap map;
	variable* ptrs[params.size()];

	for (unsigned int i = 0; i < params.size(); i++) {
		ptrs[i] = setvar(method->isnative() ? EMPTY : method->getparams()[i], trim(params[i]), map);
	}

	if (method->isnative()) {
		call_native(method->getname(), params.size(), ptrs);
	} else {
		for (unsigned int i = 0; i < lines.size(); i++) {
			variable* var = nullptr;

			ReturnType type;

			try {
				type = execline(method, &i, var, map);
			} catch (exception& e) {
				printerror(string(e.what()) + ", line #" + itos(i));
				break;
			}

			if (type == ReturnType::RETURN && var != nullptr) {
				unset(params.size(), ptrs, map);
				return var;
			}
		}
	}

	unset(params.size(), ptrs, map);
	return nullptr;
}

ReturnType execline(Method* method, unsigned int* i, variable*& var, stackmap& map) {
	wstring untrimmed = method->getlines()[*i].second;

	wstring line = trim(untrimmed);

	if (is_comment(line)) {
		printverbose(line, verbose_mode::COMMENT);
		return ReturnType::NONE;
	}

	unsigned int firstsep = line.find_first_of(KEYWORD_SEPARATOR);
	wstring keyword = firstsep == wstring::npos ? line : line.substr(0, firstsep);
	line = line.substr(firstsep + 1, line.length());

	if (line == keyword) {
		line = EMPTY;
	}

	printverbose(L"Executing keyword " + color(VERBOSE_HL) + keyword + color(VERBOSE) + L", line #" + itow(*i) + L" " + color(VERBOSE_HL)  + line );

	if (keyword == get_kw(KW_CALL_METHOD)) {
		variable* returned = invoke(line);

		if (returned != nullptr) {
			printverbose(color(COLOR_MAGENTA) + L"Returned value " + returned->get());
		}
	} else if (keyword == get_kw(KW_PRINT)) {
		wcout << parse_set_statement(line, map) << endl;
	} else if (keyword == get_kw(KW_PRINT_ERR)) {
		wcerr << color(ERROR_COLOR) << parse_set_statement(line, map) << reset() << endl;
	} else if (keyword == get_kw(KW_GOTO)) {
		linemap lines = method->getlines();

		for (unsigned int l = 0; l < lines.size(); l++) {
			if (lines[l].second == get_kw(KW_LABEL) + L" " + line)  {
				*i = l + 1;
				return ReturnType::NONE;
			}
		}
	} else if (keyword == get_kw(KW_IF)) {
		ReturnType type = parseif(method, line, i, var, map);
		return type;
	} else if (keyword == get_kw(KW_WHILE)) {
		ReturnType type = parsewhile(method, line, i, var, map);
		return type;
	} else if (keyword == get_kw(KW_FOR) || keyword == get_kw(KW_FOREACH)) {
		ReturnType type = parsefor(method, line, i, var, map, keyword == get_kw(KW_FOREACH));
		return type;
	} else if (keyword == get_kw(KW_HALT)) {
		exit(get_exit_code(line));
	} else if (keyword == get_kw(KW_BREAK)) {
		return ReturnType::BREAK;
	} else if (keyword == get_kw(KW_RETURN)) {
		if (line.length() > 0) {
			printverbose(L"Returning " + color(VERBOSE_HL) + line);
			wstring set = parse_set_statement(line, map);
			variable* v = setvar(L"temp", line, map);
			var = v;
		} else {
			var = nullptr;
			printverbose(L"Returning...");
		}
		return ReturnType::RETURN;
	} else if (keyword == get_kw(KW_CONTINUE)) {
		return ReturnType::CONTINUE;
	} else if (keyword == get_kw(KW_UNSET_VAR)) {
		unset(trim(line), map);
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

		int firstb = name.find(L"(");
		int lastb = name.find(L")");

		if (firstb != (signed int) string::npos && lastb != (signed int) string::npos) {
			wstring key = name.substr(firstb + 1, lastb - firstb - 1);
			name = name.substr(0, firstb);

			if (trim(key).length() != 0) {
				statement = key + L" > " + statement;
			}

			t = type::ARRAY;
		}

		setvar(name, statement, map, t);
	} else if (keyword == get_kw(KW_LABEL)) {
		printverbose(L"Ignoring label at line #" + itow(*i));
	} else if (keyword == get_kw(KW_READ)) {
		wstring s;

		getline(wcin, s);

		if (line.length() > 0) {
			vector<wstring> vars = split(line, ' ');

			for (wstring var : vars) {
				setvar(var, s, map);
			}
		}
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

		setvar(name, statement, map);
	} else if (keyword == get_kw(KW_ARRAY)) {
		int f = line.find_first_of(L" ");

		wstring name;
		wstring statement;

		if (f != (signed int) string::npos) {
			name = line.substr(0, f);
			statement = line.substr(f);
		} else {
			name = line;
		}

		setarr(name, statement, map);
	} else if (keyword == get_kw(KW_USE)) {
		wstring find = L" " + get_kw(KW_IMPORT_FROM) + L" ";
		int from = line.find(find);

		if (from == (signed int) string::npos) {
			import_native(line);
		} else {
			wstring file = line.substr(from + find.length());
			line = line.substr(0, from);

			import_native(line, file);
		}
	} else if (keyword == get_kw(KW_IMPORT)) {
		wstring find = L" " + get_kw(KW_IMPORT_FROM) + L" ";
		int from = line.find(find);

		if (from == (signed int) string::npos) {
			Code code(wtos(line));
			parse(code);
		} else {
			wstring file = line.substr(from + find.length());
			line = line.substr(0, from);

			Code code(wtos(file));

			vector<wstring> v = split(line, ',');

			for (wstring w : v) {
				w = trim(w);
				printverbose(L"Loading " + w + L" from " + file);
				parse(code, w);
			}
		}
	} else if (keyword == L"native") { // Test calling native functions
		invoke(line, true);
	} else {
		printerror(L"Unknown instruction " + color(ERROR_HL) + keyword + L" (" + line + L")" + color(ERROR_COLOR) + L" on line #" + itow(*i));
	}

	return ReturnType::NONE;
}

ReturnType parsefor(Method* method, wstring line, unsigned int* i, variable*& var, stackmap& map, bool range) {
	printverbose(L"Checking " + color(VERBOSE_HL) + L"for" + color(VERBOSE) + L", condition " + color(VERBOSE_HL) + line);

	int totalend;

	vector<wstring> keywords;

	vector<Chunk> chunks = parse_chunks(method, i, &totalend, range ? get_kw(KW_FOREACH) : get_kw(KW_FOR));

	variable* from = nullptr;
	variable* key = nullptr;
	arrays* arr = nullptr;
	bool isnew = false;

	for (Chunk chunk : chunks) {
		wstring line = trim(method->getlines()[chunk.start].second);

		wstring cond = line.substr(line.find_first_of(L" ") + 1);
		vector<wstring> spl = split(cond, ';');

		wstring first = cond.substr(0, cond.find_first_of(L";"));
		if (first[0] == get_kw(KW_VAR_SIGN_KEY, KW_VAR_SIGN)[0]) {
			first = first.substr(1);
		}

		array_t::iterator iter;

		// Normal for loop
		int to = 0;
		int f = 0;
		wstring dos;

		if (range) {
			first = cond.substr(0, cond.find_first_of(L"in") - 1);
			wstring arraydecl = trim(cond.substr(cond.find_first_of(L"in") + 2));

			if (is_array_expr(arraydecl)) {
				arr = setarr(EMPTY, arraydecl, map);
				isnew = true;
			} else {
				arr = (arrays*) getvar(arraydecl, map);
			}

			if (arr == nullptr) {
				throw runtime_error("No array to iterate found");
			}

			iter = arr->var.begin();

			int f = first.find(get_kw(PAIR_SEPARATOR));

			if (f != (signed int) string::npos) {
				wstring skey = trim(first.substr(0, f - 1));
				wstring value = trim(first.substr(f + get_kw(PAIR_SEPARATOR).length()));

				key = setvar(skey, iter->first, map);
				from = setvar(value, iter->second, map);
			} else {
				from = setvar(first, iter->second, map);
			}
		} else {
			for (unsigned int in = 1; in < spl.size(); in++) {
				wstring s = trim(spl[in]);
				wstring word = s.substr(0, s.find_first_of(' '));
				wstring w = s.substr(s.find_first_of(' ') + 1);

				if (startswith(s, get_kw(KW_FOR_FROM))) {
					from = setvar(first, w, map);
				} else if (startswith(s, get_kw(KW_FOR_TO))) {
					to = wtoi(w);
				} else if (startswith(s, get_kw(KW_FOR_DO))) {
					dos = w;
				} else {
					printwarning(L"Unknown " + s);
				}
			}
		}

		if (from == nullptr) {
			throw runtime_error("From variable is missing");
		}

		f = wtoi(from->get());

		if (range || f != to) {
			*i = chunk.start + 1;
			exec:

			ReturnType type = execrange(method, i, chunk.end, var, map);
			if (type == ReturnType::BREAK) {
				continue;
			} else if (range || f != to || type == ReturnType::CONTINUE) {
				*i = chunk.start + 1;

				if (range) {
					if (++iter == arr->var.end()) {
						break;
					}

					if (key != nullptr) {
						setvar(key->getname(), iter->first, map);
					}

					setvar(from->getname(), iter->second, map);
				} else {
					variable* temp = setvar(from->getname(), dos, map);
					f = wtoi(temp->get());
				}

				goto exec;
			} else if (type == ReturnType::RETURN && var != nullptr) {
				if (isnew) {
					unset(arr, map);
				}

				unset(key, map);
				unset(from, map);

				return type;
			}
			break;
		}
	}
	*i = totalend;

	if (isnew) {
		unset(arr, map);
	}

	unset(from, map);
	unset(key, map);

	return ReturnType::NONE;
}

ReturnType parsewhile(Method* method, wstring line, unsigned int* i, variable*& var, stackmap& map) {
	printverbose(L"Checking " + color(VERBOSE_HL) + L"while" + color(VERBOSE) + L", condition " + color(VERBOSE_HL) + line);

	int totalend;

	vector<Chunk> chunks = parse_chunks(method, i, &totalend, get_kw(KW_WHILE));

	for (Chunk chunk : chunks) {
		wstring line = trim(method->getlines()[chunk.start].second);

		wstring cond = line.substr(line.find_first_of(L" L") + 1);

		if (check_cond(cond, map)) {
			*i = chunk.start + 1;
			exec:
			ReturnType type = execrange(method, i, chunk.end, var, map);

			if (type == ReturnType::BREAK) {
				*i = chunk.end;
				continue;
			} else if (type == ReturnType::RETURN && var != nullptr) {
				return type;
			} else if (check_cond(cond, map) || type == ReturnType::CONTINUE) {
				*i = chunk.start + 1;
				goto exec;
			}
			break;
		}
	}
	*i = totalend;

	return ReturnType::NONE;
}

ReturnType parseif(Method* method, wstring line, unsigned int* i, variable*& var, stackmap& map) {
	printverbose(L"Checking " + color(VERBOSE_HL) + L"if" + color(VERBOSE) + L", condition " + color(VERBOSE_HL) + line);

	int totalend;

	wstring open = get_kw(KW_IF);

	vector<wstring> separators;
	separators.push_back(get_kw(KW_ELSEIF));
	separators.push_back(get_kw(KW_ELSE));

	vector<Chunk> chunks = parse_chunks(method, i, &totalend, open, &separators);

	for (Chunk conds : chunks) {
		wstring line = trim(method->getlines()[conds.start].second);

		// Is else, we have passed by everything else
		if (line == get_kw(KW_ELSE)) {
			*i = conds.start + 1;
			ReturnType type = execrange(method, i, conds.end, var, map);
			if (type == ReturnType::RETURN) {
				return type;
			}
			break;
		}

		wstring cond = line.substr(line.find_first_of(L" ") + 1);

		if (check_cond(cond, map)) {
			*i = conds.start + 1;
			ReturnType type = execrange(method, i, conds.end, var, map);
			if (type != ReturnType::NONE) {
				return type;
			}
			break;
		}
	}
	*i = totalend;

	return ReturnType::NONE;
}

ReturnType execrange(Method* method, unsigned int* i, unsigned int to, variable*& var, stackmap& map) {
	//stackmap temp = map;

	for (unsigned int from = *i; from <= to; from++) {
		ReturnType type = execline(method, &from, var, map);

		if (type != ReturnType::NONE) {
			return type;
		}

	}

	//map = temp;

	return ReturnType::NONE;
}

bool check_cond(wstring line, stackmap& map) {
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

		wstring s = line.substr(first + 1, second - first - 1);

		pos = beginnext;

		bool result = check_cond_compare(s, map);

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

bool check_cond_compare(wstring cond, stackmap& map) {
	wstring splitat = EMPTY;
	bool_ops ret;

	if (contains(cond, get_kw(OP_EQUALS))) {
		splitat = get_kw(OP_EQUALS);
		ret = bool_ops::EQUALS;
	} else if (contains(cond, get_kw(OP_NOT_EQ))) {
		splitat = get_kw(OP_NOT_EQ);
		ret = bool_ops::NOT_EQUAL;
	} else if (contains(cond, get_kw(OP_LESS_THAN))) {
		splitat = get_kw(OP_LESS_THAN);
		ret = bool_ops::LESS_THAN;
	} else if (contains(cond, get_kw(OP_MORE_THAN))) {
		splitat = get_kw(OP_MORE_THAN);
		ret = bool_ops::MORE_THAN;
	} else if (contains(cond, get_kw(OP_LESS_OR_EQUALS))) {
		splitat = get_kw(OP_LESS_OR_EQUALS);
		ret = bool_ops::LESS_OR_EQUALS;
	} else if (contains(cond, get_kw(OP_MORE_OR_EQUALS))) {
		splitat = get_kw(OP_MORE_OR_EQUALS);
		ret = bool_ops::MORE_OR_EQUALS;
	} else if (contains(cond, get_kw(KW_IN))) {
		splitat = get_kw(KW_IN);
		ret = bool_ops::IN_ARRAY;
	} else if (contains(cond, get_kw(KW_NOT_IN))) {
		splitat = get_kw(KW_NOT_IN);
		ret = bool_ops::NOT_IN_ARRAY;
	} else {
		ret = bool_ops::SINGLE;
	}

	int index = cond.find(splitat);

	wstring stat1 = parse_set_statement(trim(cond.substr(0, index)), map);
	wstring stat2 = trim(cond.substr(index + splitat.length()));

	if (ret != bool_ops::IN_ARRAY && ret != bool_ops::NOT_IN_ARRAY) {
		stat2 = parse_set_statement(stat2, map);
	}

	if (stat1.length() == 0) {
		stat1 = stat2;
		stat2 = EMPTY;
	}

	return check_cond_compare(stat1, stat2, ret, map);
}

bool check_cond_compare(const wstring& var1, const wstring& var2, bool_ops ret, stackmap& map) {
	if (ret == bool_ops::IN_ARRAY || ret == bool_ops::NOT_IN_ARRAY) {
		arrays* arr;
		bool isnew = false;
		if (is_array_expr(var2)) {
			arr = setarr(EMPTY, var2, map);
			isnew = true;
		} else {
			arr = (arrays*) getvar(var2, map);
		}

		wstring expr = parse_set_statement(var1, map);

		if (arr->var.size() > 0 && arr != nullptr) {
			for (array_t::iterator iter = arr->var.begin(); iter != arr->var.end(); ++iter) {
				if (iter->first == expr || iter->second == expr) {
					if (isnew) {
						unset(arr, map);
					}

					return ret == bool_ops::IN_ARRAY;
				}
			}
		}
		if (isnew) {
			unset(arr, map);
		}

		return ret == bool_ops::NOT_IN_ARRAY;
	}

	if (ret == bool_ops::SINGLE) {
		return var1 == get_kw(KW_TRUE);
	} else if (ret == bool_ops::EQUALS) {
		return var1 == var2;
	} else if (ret == bool_ops::NOT_EQUAL) {
		return var1 != var2;
	}

	double i1 = wtod(var1);
	double i2 = wtod(var2);

	if (ret == bool_ops::LESS_THAN) {
		return i1 < i2;
	} else if (ret == bool_ops::MORE_THAN) {
		return i1 > i2;
	} else if (ret == bool_ops::LESS_OR_EQUALS) {
		return i1 <= i2;
	} else if (ret == bool_ops::MORE_OR_EQUALS) {
		return i1 >= i2;
	}

	return DEFAULT_COND;
}

variable* getvar(wstring name, stackmap& stack) {
	if (name[0] == get_kw(KW_VAR_SIGN_KEY, KW_VAR_SIGN)[0]) {
		name = name.substr(1);
	}

	for (unsigned int i = 0; i < stack.size(); i++) {
		variable* v = stack[i];
		if (v->getname().length() > 0 && v->getname() == name) {
			return stack[i];
		}
	}

	return nullptr;
}

wstring parse_set_statement(wstring s, stackmap& map) {
	printverbose(L"Checking variable set statement " + color(VERBOSE_HL) + s);

	if (s.length() == 0) {
		return s;
	}

	s = trim(s);

	bool opposite = s[0] == L'!';
	bool var = s[opposite ? 1 : 0] == get_kw(KW_VAR_SIGN_KEY, KW_VAR_SIGN)[0];

	ExprType type = ExprType::NONE;

	wstring expr = rm_strings(s);

	if (is_string_expr(expr)) {
		vector<wstring> vec = split_ignore_strings(s, get_kw(OP_CONCATENATE_STRINGS));

		if (vec.size() > 0) {
			s = EMPTY;
		}

		for (wstring w : vec) {
			s += parse_set_statement(w, map);
		}
	} else if (is_bool_expr(expr)) {
		type = ExprType::BOOLEAN;
	} else if (is_math_expr(expr)) {
		type = ExprType::MATH;
	}

	bool startstr = s[0] == L'\"' && s[s.length() - 1] == L'\"';

	if (startstr) {
		s = s.substr(1, s.length() - 2);
	}

	// Single variable
	if (var && s.find(L" ") == wstring::npos) {
		wstring name = s.substr(opposite ? 2 : 1);

		variable* v = getvar(name, map);
		if (v != nullptr && v->getname() == name) {
			if (opposite) {
				bool yes = v->get() == get_kw(KW_TRUE);

				s = yes ? get_kw(KW_FALSE) : get_kw(KW_TRUE);
			} else {
				s = v->get();
			}
		}

	} else if (s != get_kw(KW_TRUE) && s != get_kw(KW_FALSE)) {
		s = parsevars(s, map);

		if (type == ExprType::MATH) {
			s = replaceAll(s, L" ", EMPTY);
			double val = eval(wtos(s));

			s = itow(val);

			int f = s.find_last_of('.');

			if (f != (signed int) wstring::npos && s.substr(f + 1).find_first_not_of('0') == wstring::npos) {
				// If decimal value, remove dot and zeros
				s = s.substr(0, f);
			}
		} else if (type == ExprType::BOOLEAN) {
			bool result = check_cond(s, map);
			s = result ? get_kw(KW_TRUE) : get_kw(KW_FALSE);
		}
	}

	return s;
}

arrays* setarr(wstring name, wstring statement, stackmap& stack) {
	vector<wstring> statements;

	statement = trim(statement);

	if (statement.length() > 0) {
		statements = split(statement.substr(1, statement.length() - 3), ',');
	}

	return (arrays*) setvar(name, statements, stack, type::ARRAY);
}

variable* setvar(wstring statement, stackmap& stack, type t) {
	return setvar(EMPTY, statement, stack, t);
}

variable* setvar(wstring name, wstring statement, stackmap& stack, type t) {
	vector<wstring> statements;
	statements.push_back(statement);
	return setvar(name, statements, stack, t);
}

variable* setvar(wstring name, vector<wstring> statements, stackmap& stack, type t) {
	if (name[0] == get_kw(KW_VAR_SIGN_KEY, KW_VAR_SIGN)[0]) {
		name = name.substr(1);
	}

	variable* var = nullptr;

	int index = -1;
	for (unsigned int k = 0; k < stack.size(); k++) {
		variable* v = stack[k];
		if (v->getname() == name) {
			var = v;
			index = k;
			break;
		}
	}

	if (t == type::DEFAULT) {
		if (name == EMPTY || var == nullptr) {
			var = new str(name);
		}

		var->set(parse_set_statement(statements[0], stack));
	} else if (t == type::ARRAY) {
		arrays* arr;

		if (name == EMPTY || var == nullptr) {
			arr = new arrays(name);
		} else {
			arr = (arrays*) var;
		}

		for (unsigned int i = 0; i < statements.size(); i++) {
			wstring expr = statements[i];

			wstring key;
			wstring statement;

			int f = expr.find(get_kw(PAIR_SEPARATOR));

			if (f != (signed int) string::npos) {
				key = trim(expr.substr(0, f - 1));
				statement = trim(expr.substr(f + get_kw(PAIR_SEPARATOR).length()));
			} else {
				key = itow(arr->var.size());
				statement = expr;
			}

			printverbose(L"Setting " + key + L" to " + statement, verbose_mode::ADDITION);

			arr->var[parse_set_statement(trim(key), stack)] = parse_set_statement(trim(statement), stack);
		}

		var = arr;
	}

	if (index != -1) {
		printverbose(L"Updated " + color(VERBOSE_HL) + gettext(var) + color(VERBOSE) + L" on stack with value " + color(VERBOSE_HL) +var->get(), verbose_mode::ADDITION);
		stack.at(index) = var;
	} else {
		printverbose(L"Added " + color(VERBOSE_HL) + gettext(var) + color(VERBOSE) + L" to stack with value " + color(VERBOSE_HL)+ var->get(), verbose_mode::ADDITION);

		stack.insert(stack.begin(), var);
	}

	return var;
}

bool is_bool_expr(const wstring& expr) {
	return contains(expr, get_kw(OP_EQUALS))
		|| contains(expr, get_kw(OP_NOT_EQ))
		|| contains(expr, get_kw(OP_LESS_THAN))
		|| contains(expr, get_kw(OP_MORE_THAN))
		|| contains(expr, get_kw(OP_LESS_OR_EQUALS))
		|| contains(expr, get_kw(OP_MORE_OR_EQUALS))
		|| contains(expr, get_kw(KW_AND))
		|| contains(expr, get_kw(KW_OR))
		|| contains(expr, get_kw(KW_XOR))
		|| contains(expr, get_kw(OP_OR))
		|| contains(expr, get_kw(OP_XOR))
		|| contains(expr, get_kw(OP_AND))
		|| contains(expr, get_kw(KW_IN))
		|| contains(expr, get_kw(KW_NOT_IN));
}

bool is_math_expr(const wstring& expr) {
	return contains(expr, get_kw("+"))
		|| contains(expr, get_kw("-"))
		|| contains(expr, get_kw("/"))
		|| contains(expr, get_kw("%"))
		|| contains(expr, get_kw("*"));
}

bool is_array_expr(const wstring& expr) {
	return expr[0] == '[' && expr[expr.length() - 1] == ']';
}

bool is_string_expr(const wstring& expr) {
	bool startstr = expr[0] == L'\"' && expr[expr.length() - 1] == L'\"';

	return startstr || contains(expr, get_kw(OP_CONCATENATE_STRINGS));
}

inline void unset(wstring name, stackmap& stack) {
	unset(getvar(name, stack), stack);
}

void unset(int count, variable* vars[], stackmap& stack) {
	for (int i = 0; i < count; i++) {
		unset(vars[i], stack);
	}
}

void unset(variable* var, stackmap& stack) {
	if (var != nullptr) {
		for (unsigned int i = 0; i < stack.size(); i++) {
			variable* var2 = stack[i];

			if (var2->getname() == var->getname()) {
				stack.erase(stack.begin() + i);
				break;
			}
		}

		printverbose(L"Deleting " + gettext(var), verbose_mode::DELETION);

		delete var;
	}
}

inline bool is_comment(wstring s) {
	return startswith(s, COMMENT_1) || startswith(s, COMMENT_2);
}
