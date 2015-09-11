#include <string>
#include <iostream>
#include <stdlib.h>
#include <sstream>

#include "headers/utils.h"
#include "headers/colors.h"
#include "headers/keywords.h"
#include "headers/code.h"
#include "headers/engine.h"
#include "headers/var.h"

using namespace std;

extern bool verbose;

static const wchar_t* TRIM = L"\t\r\n ";

bool startswith(string s, string s1) {
	return s.substr(0, s1.size()) == s1;
}

bool contains(string s, string find) {
	return s.find(find) != wstring::npos;
}

bool startswith(wstring s, wstring s1) {
	return s.substr(0, s1.size()) == s1;
}

bool contains(wstring s, wstring find) {
	return s.find(find) != wstring::npos;
}

wstring trim(wstring s) {
	int first = s.find_first_not_of(TRIM);
	int last = s.find_last_not_of(TRIM);

	if (s.length() == 0 || first == -1 || last == -1) {
		return L"";
	}

	s = s.substr(first, last + 1);

	return s;
}

void printverbose(wstring s, verbose_mode mode) {
	if (verbose) {
		wstring c;

		if (mode == verbose_mode::ADDITION) {
			c = color(COLOR_GREEN) + L"+ ";
		} else if (mode == verbose_mode::DELETION) {
			c = color(COLOR_RED) + L"- ";
		} else if (mode == verbose_mode::COMMENT) {
			c = color(COLOR_COMMENT) + L"# ";
			s = color(COLOR_COMMENT) + trim(s.substr(1));
		} else {
			c = L"* ";
		}
		wcout << c << color(VERBOSE) << s << reset() << endl;
	}
}

void printerror(string s) {
	cout << colorstr(ERROR_COLOR) << "ERROR: " << s << resetstr() << endl;
}

void printwarning(string s) {
	cout << colorstr(WARNING) << "WARNING: " << s << resetstr() << endl;
}

void printerror(wstring s) {
	wcout << color(ERROR_COLOR) << L"ERROR: " << s << reset() << endl;
}

void printwarning(wstring s) {
	wcout << color(WARNING) << L"WARNING: " << s << reset() << endl;
}

wstring replaceAll(wstring str, wstring from, wstring to) {
    if(from.empty())
        return str;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != wstring::npos) {
    	if (str[start_pos - 1] != L'\\') {
            str = str.replace(start_pos, from.length(), to);
            start_pos += to.length();
    	} else {
    		str = str.replace(start_pos - 1, 1, L"");
    		start_pos++;
    	}
    }

    return str;
}

vector<wstring> split(wstring s, wchar_t delim) {
	vector<wstring> elems;
	wstringstream ss(s);
	wstring item;
	while (getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}

int get_exit_code(wstring s) {
	if (s == L"normal") {
		return EXIT_SUCCESS;
	} else if (s == L"error") {
		return EXIT_FAILURE;
	} else {
		return atoi(string(s.begin(), s.end()).c_str());
	}
}

wstring itow(int i) {
	return to_wstring(i);
}

string itos(int i) {
	return to_string(i);
}

int wtoi(wstring w) {
	return atoi(string(w.begin(), w.end()).c_str());
}

double wtod(wstring w) {
	return atof(string(w.begin(), w.end()).c_str());
}

wstring dtow(double d) {
	return to_wstring(d);
}

wstring stow(string s) {
	return wstring(s.begin(), s.end());
}

string wtos(wstring w) {
	return string(w.begin(), w.end());
}

wstring remove_brackets(wstring w) {
	if (w.length() <= 1) {
		return w;
	}
	if (w.substr(0, 1) == get_kw(KW_END)) {
		w = w.substr(1);
	}

	if (w.substr(w.length() - 1) == get_kw(KW_OPEN)) {
		w = w.substr(0, w.length() - 1);
	}



	return w;
}
