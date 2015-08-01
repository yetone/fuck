#include <string>

#include "headers/method.h"

using namespace std;

Method::Method(wstring name): name(name) {

}

Method::Method(wstring ns, wstring name): ns(ns), name(name) {

}

Method::Method(wstring ns, wstring name, vector<wstring>& lines): ns(ns), name(name), lines(lines) {

}

vector<wstring>& Method::getlines() {
	return this->lines;
}

const wstring& Method::getname() {
	return this->name;
}

const wstring& Method::getnamespace() {
	return this->ns;
}

const wstring Method::getdisplayname() {
	if (this->ns.length() == 0) {
		return this->name;
	} else {
		return this->ns + L"." + this->name;
	}
}
