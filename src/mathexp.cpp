#include <string>

#include "headers/utils.h"
#include "headers/colors.h"

using namespace std;

int eval(string s) {
	printverbose("Evaluating mathematical expression " + color(VERBOSE_HL) + s);

	int val;

	val = stoi(s);

	return val;
}
