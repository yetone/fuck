#include <string>
#include <iostream>
#include <stdlib.h>
#include <sstream>

#include "headers/utils.h"
#include "headers/colors.h"
#include "headers/keywords.h"
#include "headers/code.h"
#include "headers/var.h"
#include "headers/parser.h"

using namespace std;

extern bool verbose;

extern methodmap methodMap;
extern stackmap stackMap;

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

void printverbose(string s, bool star) {
	if (verbose) {
		cout << (star ? "* " : "") << color(VERBOSE) << s << reset() << endl;
	}
}

void printerror(string s) {
	cout << color(ERROR) << "ERROR: " << s << reset() << endl;
}

void printwarning(string s) {
	cout << color(WARNING) << "WARNING: " << s << reset() << endl;
}

string parsevars(string s) {
	for (Variable v : stackMap) {
		int f = s.find("$" + v.name);

		if (f != (signed int) string::npos) {
			s = replaceAll(s, "$" + v.name, v.var);
		}
	}

	for (Method* m : methodMap) {
		string find = m->getname() + "()";

		int f = 0;

		while ((f = s.find(find, f)) != (signed int) string::npos) {
			Variable* returned = invoke(m);

			if (returned == NULL) {
				printerror("Function " + color(ERROR_HL) + m->getname() + color(ERROR) + " did not return any value");
			} else {
				s = s.replace(f, find.length(), returned->var);
				f += returned->var.length();
			}
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
    		str = str.replace(start_pos - 1, 1, "");
    		start_pos++;
    	}
    }

    return str;
}

vector<string> split(string& s, char delim) {
	vector<string> elems;
	stringstream ss(s);
	string item;
	while (getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
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
