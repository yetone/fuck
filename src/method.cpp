#include <string>

#include "headers/method.h"
#include "headers/engine.h"
#include "headers/keywords.h"

using namespace std;

Method::Method(wstring name): name(name) { }

Method::Method(wstring ns, wstring name): ns(ns), name(name) { }

Method::Method(wstring ns, wstring name, parameters params): ns(ns), name(name), params(params) { }

Method::Method(wstring ns, wstring name, linemap& lines): ns(ns), name(name), lines(lines) { }

linemap& Method::getlines() {
	return this->lines;
}

parameters& Method::getparams() {
	return this->params;
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

const bool Method::ismain() {
	return this->name == ENTRY_POINT;
}
