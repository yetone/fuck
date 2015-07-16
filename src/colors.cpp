#include "headers/colors.h"

extern bool colors;

string reset() {
	return colors ? COLOR_RESET : "";
}

string color(string color) {
	return colors ? color : "";
}
