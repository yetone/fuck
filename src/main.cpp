#include <iostream>
#include <cstring>
#include <fstream>

#include "headers/keywords.h"
#include "headers/utils.h"
#include "headers/code.h"
#include "headers/engine.h"
#include "headers/var.h"

using namespace std;

#define NORMAL_EXIT 0
#define VERSION "0.0"

#define ARG_VERBOSE "--verbose"
#define ARG_VERBOSE_SHORT "-v"
#define ARG_COLORS "--colors"
#define ARG_COLORS_SHORT "-c"
#define EXIT L"exit"

bool verbose = false;
bool colors = false;
methodmap methodMap;
stackmap stackMap;

void printinfo() {
	cout << "fulang " << VERSION << endl;
}

int main(int argc, char* argv[]) {
	cout.setf(ios::boolalpha);

	if (argc <= 1) {
		printerror("No input files specified");
		return 1;
	}

	// If we has gotten any files to parse
	bool gotfiles = false;

	vector<string> files;

	// Skip first in argument list
	for (int i = 1; i < argc; i++) {
		if (!strcmp(argv[i], ARG_VERBOSE) || !strcmp(argv[i], ARG_VERBOSE_SHORT)) {
			verbose = true;
		} else if (!strcmp(argv[i], ARG_COLORS)|| !strcmp(argv[i], ARG_COLORS_SHORT)) {
			colors = true;
		} else if (!startswith(argv[i], "-")){
			gotfiles = true;

			string file = argv[i];

			wifstream input(argv[i]);

			if (!input.good()) {
				printerror("Failed to read file " + file);
				input.close();
				continue;
			}

			input.close();

			files.push_back(file);
		}
	}

	for (string file : files) {
		Code code(file);

		parse(code);
	}

	if (gotfiles) {
		// We have processed all input files, exit
		return NORMAL_EXIT;
	}

	printinfo();

	Method main(L"main");
	unsigned int i = 0;

	while (true) {
		cout << ">> ";

		wstring s;
		getline(wcin, s);

		if (s == EXIT) {
			return NORMAL_EXIT;
		}

		Variable *var;

		main.lines.push_back(s);

		execline(&main, &i, 0, var);
		i++;
	}

	return NORMAL_EXIT;
}
