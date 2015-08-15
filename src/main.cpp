#include <iostream>
#include <cstring>
#include <fstream>
#include <utility>

#include "headers/keywords.h"
#include "headers/utils.h"
#include "headers/code.h"
#include "headers/engine.h"
#include "headers/parser.h"
#include "headers/var.h"
#include "headers/main.h"

using namespace std;

bool verbose = false;
bool colors = false;
methodmap methodMap;

void printinfo() {
	cout << "fulang, version " << VERSION << endl;
}

void printusage() {
	printinfo();

	cout << "Usage:\tfulang [options] script ..." << endl;
	cout << "Options:" << endl;
	cout << "\t--help" << endl;
	cout << "\t-c, --colors" << endl;
	cout << "\t-v, --verbose" << endl;
}

int main(int argc, char* argv[]) {
	cout.setf(ios::boolalpha);

	// If we has gotten any files to parse
	bool gotfiles = false;

	vector<string> files;

	// Skip first in argument list
	for (int i = 1; i < argc; i++) {
		if (!strcmp(argv[i], ARG_VERBOSE) || !strcmp(argv[i], ARG_VERBOSE_SHORT)) {
			verbose = true;
		} else if (!strcmp(argv[i], ARG_COLORS) || !strcmp(argv[i], ARG_COLORS_SHORT)) {
			colors = true;
		} else if (!strcmp(argv[i], ARG_HELP)) {
			printusage();
			exit(0);
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

	Method main(ENTRY_POINT);

	vector<wstring> lines;

	while (true) {
		cout << LINE_PREFIX;

		wstring s;
		getline(wcin, s);

		if (s == COMMAND_EXIT) {
			return NORMAL_EXIT;
		} else if (s == COMMAND_RUN) {
			Code code("", lines);
			parse(code);
		} else {
			lines.push_back(s);
		}
	}

	return NORMAL_EXIT;
}
