#include <iostream>
#include <cstring>

#include "keywords.h"
#include "parser.h"

using namespace std;

bool verbose = false;

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
			if (verbose) {
				cout << "Parsing file " << argv[i] << endl;
			}
			Code code(argv[i]);

			methodmap* methods = parse(&code);

			run(methods);
		}
	}

	return 0;
}
