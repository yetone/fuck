#include <string>
#include <stdexcept>

#include "headers/var.h"
#include "headers/utils.h"

using namespace std;

wstring& variable::getraw() {
	return this->name;
}

wstring variable::at(const int& i) {
	wchar_t c = this->name[i];

	return wstring(1, c);
}

wstring variable::at(const wstring& w) {
	throw runtime_error("Can't get data at index from string in normal variable");
}


wstring array::at(const int&) {
	return this->name;
}

wstring array::at(const wstring&) {
	return this->name;
}
