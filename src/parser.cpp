#include <vector>
#include <string>

#include "headers/engine.h"
#include "headers/parser.h"
#include "headers/utils.h"
#include "headers/code.h"
#include "headers/keywords.h"
#include "headers/colors.h"

using namespace std;

extern methodmap methodMap;
extern stackmap stackMap;

vector<Chunk> parse_chunks(Method* method, unsigned int* i, int indent, int* totalend, vector<wstring> keywords) {
	unsigned int end = *i;

	wstring line;

	vector<Chunk> chunks;

	Chunk *current = nullptr;

	while (end < method->getlines().size()) {
		wstring temp = method->getlines()[end];

		int s = temp.find_first_not_of('\t');

		bool word = false;

		for (wstring str : keywords) {
			if (startswith(trim(temp), get_kw(str))) {
				word = true;
			}
		}

		if (s == indent && word) {
			if (current != nullptr) {
				current->end = end - 1;
				chunks.push_back(*current);
			}

			current = new Chunk;
			current->start = end;
			*i = end;
		} else if (s == indent && startswith(trim(temp), get_kw(KW_END))) {
			if (current != nullptr) {
				line = method->getlines()[current->start];
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


wstring parsevars(wstring s) {
	int f = 0;
	while ((f = s.find(L"[", f)) != (signed int) wstring::npos) {
		int f2 = s.find(L"]", f);

		wstring replace = s.substr(f, f2 - f + 1);
		wstring expr = trim(replace.substr(1, replace.length() - 2));

		if (startswith(expr, get_kw(KW_EXPR))) {
			expr = expr.substr(5);

			expr = parse_set_statement(expr);
		} else {
			throw runtime_error("No action specified");
		}

		s = s.replace(f, replace.length(), expr);
		f += expr.length();
	}

	for (variable* v : stackMap) {
		if (v->gettype() == type::ARRAY) {
			array_t* map = v->getpairs();

			for (array_t::iterator it = map->begin(); it != map->end(); ++it) {
				wstring replace = get_kw(KW_VAR_SIGN_KEY, KW_VAR_SIGN) + v->name + L"(" + it->first + L")";

				unsigned int f = s.find(replace);

				if (f != wstring::npos) {
					s = replaceAll(s, replace, it->second);
				}
			}
		}

		wstring replace = get_kw(KW_VAR_SIGN_KEY, KW_VAR_SIGN) + v->name;
		unsigned int f = s.find(replace);

		if (f != wstring::npos) {
			s = replaceAll(s, replace, v->get());
		}
	}

	for (Method* m : methodMap) {
		wstring find = m->getname() + L"()";

		int f = 0;

		while ((f = s.find(find, f)) != (signed int) wstring::npos) {
			variable* returned = invoke(m);

			if (returned == nullptr) {
				printerror(L"Function " + color(ERROR_HL) + m->getname() + color(ERROR_COLOR) + L" did not return any value");
			} else {
				s = s.replace(f, find.length(), returned->get());
				f += returned->get().length();
			}
		}
	}

	return s;
}
