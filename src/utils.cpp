#include <string>
#include <iostream>

#include "utils.h"
#include "colors.h"

using namespace std;

extern bool colors;

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
		cout << (colors ? COLOR_LIGHT_YELLOW : "") << s << (colors ? COLOR_RESET : "") << endl;
	}
}

void err(string s) {
	cerr << (colors ? COLOR_LIGHT_RED : "") << s << (colors ? COLOR_RESET : "") << endl;
}
