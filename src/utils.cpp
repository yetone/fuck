#include <string>
#include <iostream>
#include <stdlib.h>

#include "headers/utils.h"
#include "headers/colors.h"
#include "headers/keywords.h"
#include "headers/code.h"
#include "headers/var.h"

using namespace std;

extern bool verbose;
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
		cout << "* " << color(VERBOSE) << s << reset() << endl;
	}
}

void printerror(string s) {
	cout << color(ERROR) << "ERROR: " << s << reset() << endl;
}

void err(string s) {
	cerr << color(ERROR) << s << reset() << endl;
}

string parsevars(string s) {
	for (defvar v : stackMap) {
		int f = s.find("$" + v.name);

		if (f != (signed int) string::npos) {
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
    	if (str[start_pos - 1] != '\\') {
            str = str.replace(start_pos, from.length(), to);
            start_pos += to.length();
    	} else {
    		start_pos++;
    	}
    }

    return str;
}

int get_exit_code(string s) {
	if (s == "normal") {
		return EXIT_SUCCESS;
	} else if (s == "error") {
		return EXIT_FAILURE;
	} else {
		return atoi(s.c_str());
	}
}
