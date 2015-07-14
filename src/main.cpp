#include <iostream>

#include "keywords.h"
#include "parser.h"

using namespace std;

#define VERBOSE

int main(int argc, char* argv[]) {

	if (argc <= 1) {
		cerr << "No input files specified";
		return 1;
	}

	// Skip first in argument list
	for (int i = 1; i < argc; i++) {
		#ifdef VERBOSE
		cout << "Parsing file " << argv[i] << endl;
		#endif
		Code code(argv[i]);

		parse(&code);
	}

	return 0;
}
