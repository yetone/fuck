#include <iostream>
#include <cstring>
#include <fstream>

#include "headers/keywords.h"
#include "headers/parser.h"
#include "headers/utils.h"
#include "headers/code.h"
#include "headers/var.h"

using namespace std;

#define NORMAL_EXIT 0

bool verbose = false;
bool colors = false;
methodmap methodMap;
stack stackMap;

int main(int argc, char* argv[]) {

	if (argc <= 1) {
		cerr << "No input files specified";
		return 1;
	}

	// If we has gotten any files to parse
	bool gotfiles = false;

	// Skip first in argument list
	for (int i = 1; i < argc; i++) {

		if (strcmp(argv[i], "--verbose") == 0 || strcmp(argv[i], "-v") == 0) {
			verbose = true;
		} else if (strcmp(argv[i], "--colors") == 0) {
			colors = true;
		} else if (!startswith(argv[i], "-")){
			gotfiles = true;

			string file = argv[i];

			ifstream input(argv[i]);

			if (!input.good()) {
				printerror("Failed to read file " + file);
				input.close();
				continue;
			}

			input.close();

			Code code(argv[i]);

			parse(&code);
		}
	}

	if (gotfiles) {
		return NORMAL_EXIT;
	}



	return NORMAL_EXIT;
}
