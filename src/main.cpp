#include <iostream>
#include <cstring>

#include "keywords.h"
#include "parser.h"
#include "utils.h"

using namespace std;

bool verbose = false;
bool colors = false;
methodmap methodMap;

int main(int argc, char* argv[]) {

	if (argc <= 1) {
		cerr << "No input files specified";
		return 1;
	}

	// Skip first in argument list
	for (int i = 1; i < argc; i++) {

		if (strcmp(argv[i], "--verbose") == 0 || strcmp(argv[i], "-v") == 0) {
			verbose = true;
		} else if (strcmp(argv[i], "--colors") == 0) {
			colors = true;
		} else if (!startswith(argv[i], "-")){
			Code code(argv[i]);

			parse(&code);
		}
	}

	return 0;
}
