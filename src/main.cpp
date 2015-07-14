#include <iostream>

#include "keywords.h"
#include "parser.h"
#include "code.h"

using namespace std;

int main(int argc, char* argv[]) {

	if (argc <= 1) {
		cerr << "No input files specified";
		return 1;
	}

	// Skip first in argument list
	for (int i = 1; i < argc; i++) {
		Code code(argv[i]);
	}

	return 0;
}
