#include <string>
#include <iostream>

#include "utils.h"
#include "colors.h"
#include "keywords.h"
#include "code.h"

using namespace std;

extern bool verbose;
extern bool colors;
extern stack stackMap;

bool startswith(string s, string s1) {
	return s.substr(0, s1.size()) == s1;
}

string trim(string s) {
	int first = s.find_first_not_of(TRIM);
	int last = s.find_last_not_of(TRIM);

	if (s.length() == 0 || first == -1 || last == -1) {
		return "";
	}

	s = s.substr(first, last + 1);

	return s;
}

void printverbose(string s) {
	if (verbose) {
		cout << "* " << (colors ? COLOR_YELLOW : "") << s << (colors ? COLOR_RESET : "") << endl;
	}
}

void printerror(string s) {
	cout << (colors ? COLOR_RED : "") << "ERROR: " << s << (colors ? COLOR_RESET : "") << endl;
}

void err(string s) {
	cerr << (colors ? COLOR_RED : "") << s << (colors ? COLOR_RESET : "") << endl;
}

string parsevars(string s) {
	for (defvar v : stackMap) {
		if (s.find("$" + v.name) != string::npos) {
			s = replaceAll(s, "$" + v.name, v.var);
		}
	}

	return s;
}

string replaceAll(string str, string from, string to) {
    if(from.empty())
        return str;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != string::npos) {
        str = str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }

    return str;
}
