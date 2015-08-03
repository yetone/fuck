#include <string>
#include <stdexcept>

#include "headers/var.h"
#include "headers/utils.h"

using namespace std;

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
	return L"";
}

void arrays::set(const wstring& key, const wstring& value) {
	this->var[key] = value;
}

void arrays::set(const wstring& value) {

}
