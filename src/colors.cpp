#include "headers/colors.h"

extern bool colors;

string resetstr() {
	return colorstr(COLOR_RESET);
}

wstring reset() {
	return color(COLOR_RESET);
}

string colorstr(string color) {
	return colors ?  color : "";
}

wstring color(string color) {
	return colors ? wstring(color.begin(), color.end()) : L"";
}
