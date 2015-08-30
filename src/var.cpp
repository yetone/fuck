#include <string>
#include <stdexcept>

#include "headers/var.h"
#include "headers/utils.h"

using namespace std;

variable::variable() {
	this->name = L"";
}

variable::variable(wstring name) {
	this->name = name;
}

wstring variable::getname() {
	return this->name;
}

wstring str::get() {
	return this->var;
}

void str::set(const wstring& value) {
	this->var = value;
}

wstring arrays::get(const wstring& key) {
	return this->var[key];
}

wstring arrays::get() {
	wstring s;

	for (array_t::iterator iter = this->var.begin(); iter != this->var.end(); ++iter) {
		s += iter->first + L"=" + iter->second + L", ";
	}

	return s.substr(0, s.length() - 2);
}

void arrays::set(const wstring& key, const wstring& value) {
	this->var[key] = value;
}

void arrays::set(const wstring& value) {

}

array_t* arrays::getpairs() {
	return &this->var;
}

wstring gettext(variable* v) {
	wstring w;

	if (v->getname().length() == 0) {
		w = L"temporary variable";
	} else {
		w = v->getname();
	}

	return w;
}
