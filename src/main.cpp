#include <iostream>
#include <cstring>

#include "keywords.h"
#include "parser.h"

using namespace std;

bool verbose = false;
methodmap methodMap;

int main(int argc, char* argv[]) {

	if (argc <= 1) {
		cerr << "No input files specified";
		return 1;
	}

	// Skip first in argument list
	for (int i = 1; i < argc; i++) {

		if (strcmp(argv[i], "--verbose") == 0) {
			verbose = true;
		} else {
			Code code(argv[i]);

			parse(&code);
		}
	}

	return 0;
}
