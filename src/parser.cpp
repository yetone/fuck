#include <vector>
#include <string>
#include <cstring>

#include "headers/engine.h"
#include "headers/parser.h"
#include "headers/utils.h"
#include "headers/code.h"
#include "headers/keywords.h"
#include "headers/colors.h"

using namespace std;

extern methodmap methodMap;
#include <iostream>

void parse(Code& code, wstring what) {
	printverbose(color(COLOR_GREEN) + L"Parsing " + stow(code.getfile()));

	vector<wstring> lines = code.getlines();

	// Current namespace, might be empty
	wstring currentns = EMPTY;

	// Current method name, if no method, should be L"main"
	Method* currentmethod;
	Method* main = new Method(ENTRY_POINT);
	methodMap.push_back(main);
	currentmethod = main;

	bool one = what.length() > 0;

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
				line = EMPTY;
			}
		}

		unsigned int firstsep = line.find_first_of(KEYWORD_SEPARATOR);
		wstring keyword = firstsep == wstring::npos ? line : line.substr(0, firstsep);
		line = line.substr(firstsep + 1, line.length());

		if (keyword == get_kw(KW_NAMESPACE)) {
			if (end) {
				currentns = EMPTY;
			} else {
				currentns = line;
			}
		} else if (keyword == get_kw(KW_METHOD)) {
			if (end) {
				if (one && currentmethod != nullptr && what != currentmethod->getdisplayname()) {
					delete currentmethod;
					currentmethod = main;
					continue;
				}

				printverbose(L"Method " + color(VERBOSE_HL) + currentmethod->getdisplayname() + color(VERBOSE) + L" finished");
				methodMap.push_back(currentmethod);

				currentmethod->chunk.end = rl + 1;

				currentmethod = main;
			} else {
				int nameindex = line.find_first_of(KEYWORD_SEPARATOR);
				wstring name = trim(nameindex == (signed int) string::npos ? line : line.substr(0, nameindex));

				parameters params;

				if (nameindex == (signed int) string::npos) {
					name = trim(line);
				} else {
					name = trim(line.substr(0, nameindex));

					params = split(trim(line.substr(nameindex)), ' ');
				}

				currentmethod = new Method(currentns, name, params);

				if (one && currentmethod->getdisplayname() != what) {
					continue;
				}

				currentmethod->chunk.start = rl + 1;
				printverbose(L"Method " + color(VERBOSE_HL) + currentmethod->getdisplayname() + color(VERBOSE) + L" initialized");
			}
		} else {
			currentmethod->lines.push_back(make_pair(i, lines[i]));
		}
	}

	printverbose(color(COLOR_GREEN) + L"Parsing complete");
}

vector<Chunk> parse_chunks(Method* method, unsigned int* i, int* totalend, wstring open, vector<wstring> *separators) {
	unsigned int end = *i;

	wstring line;

	vector<Chunk> chunks;

	Chunk *current = nullptr;

	int depth = 0;

	while (end < method->getlines().size()) {
		wstring temp = trim(method->getlines()[end].second);

		bool isopen = startswith(temp, open);
		bool isseparator = false;

		if (separators != nullptr) {
			for (wstring str : *separators) {
				if (startswith(temp, get_kw(str))) {
					isseparator = true;
				}
			}
		}

		if ((isseparator && depth - 1 == 0) || (isopen && depth++ == 0)) {
			current = new Chunk;
			current->start = end;
			*i = end;
		} else if (current != nullptr && startswith(trim(temp), get_kw(KW_END))) {
			depth--;

			if (depth == 0) {
				line = method->getlines()[current->start].second;
				wstring cond = line.substr(line.find_first_of(L" ") + 1);

				current->end = end - 1;
				chunks.push_back(*current);
				*i = end;
				*totalend = end;
				break;
			}
		}

		end++;
	}

	return chunks;
}


wstring parsevars(wstring s, stackmap& map) {
	int f = 0;
	while ((f = s.find(L"[", f)) != (signed int) wstring::npos) {
		int f2 = s.find(L"]", f);

		wstring replace = s.substr(f, f2 - f + 1);
		wstring expr = trim(replace.substr(1, replace.length() - 2));

		if (startswith(expr, get_kw(KW_EXPR))) {
			expr = expr.substr(get_kw(KW_EXPR).length() + 1);

			expr = parse_set_statement(expr, map);
		} else if (startswith(expr, get_kw(KW_CALL_METHOD))) {
			expr = expr.substr(get_kw(KW_CALL_METHOD).length() + 1);

			variable *var = invoke(trim(expr));

			if (var != nullptr) {
				expr = var->get();

				unset(var, map);
			}
		}

		s = s.replace(f, replace.length(), expr);
		f += expr.length();
	}

	for (variable* v : map) {
		if (v->gettype() == type::ARRAY) {
			array_t* map = v->getpairs();

			for (array_t::iterator it = map->begin(); it != map->end(); ++it) {
				wstring replace = get_kw(KW_VAR_SIGN_KEY, KW_VAR_SIGN) + v->getname() + L"(" + it->first + L")";

				unsigned int f = s.find(replace);

				if (f != wstring::npos) {
					s = replaceAll(s, replace, it->second);
				}
			}
		}

		wstring replace = get_kw(KW_VAR_SIGN_KEY, KW_VAR_SIGN) + v->getname();
		unsigned int f = s.find(replace);

		if (f != wstring::npos) {
			s = replaceAll(s, replace, v->get());
		}
	}

	return s;
}

wstring rm_strings(wstring s) {
	int f = 0;
	while ((f = s.find(L"[", f)) != (signed int) wstring::npos) {
		int f2 = s.find(L"]", f + 1);

		wstring replace = s.substr(f, f2 - f + 1);

		s = s.replace(f, replace.length(), L"");
		f += 1;
	}

	f = 0;
	while ((f = s.find(L"\"", f)) != (signed int) wstring::npos) {
		int f2 = s.find(L"\"", f + 1);

		wstring replace = s.substr(f, f2 - f + 1);

		s = s.replace(f, replace.length(), L"");
		f += 1;
	}

	return s;
}

vector<wstring> split_ignore_strings(wstring w, wstring delim) {
	vector<wstring> vec;

	bool instr = false;

	for (unsigned int i = 0, i1 = 0; i < w.length(); i++) {
		if (w[i] == '\"' && i + 1 != w.length()) {
			instr = !instr;
		} else if ((!instr && w.substr(i, delim.length()) == delim) || i + 1 == w.length()) {
			wstring entry = w.substr(i1, i - i1);
			i1 = i + delim.length();

			vec.push_back(trim(entry));
		}
	}

	// Quick fix
	vec[vec.size() - 1] += w[w.length() - 1];

	return vec;
}
