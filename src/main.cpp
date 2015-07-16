#include <iostream>
#include <cstring>

#include "keywords.h"
#include "parser.h"
#include "utils.h"
#include "code.h"

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

			Code code(argv[i]);

			parse(&code);
		}
	}

	if (gotfiles) {
		return NORMAL_EXIT;
	}



	return NORMAL_EXIT;
}
