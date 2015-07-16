#include <string>

#include "headers/method.h"

using namespace std;

Method::Method(string name): name(name) {

}

Method::Method(string ns, string name): ns(ns), name(name) {

}

Method::Method(string ns, string name, vector<string> lines): ns(ns), name(name), lines(lines) {

}

vector<string> Method::getlines() {
	return this->lines;
}

string Method::getname() {
	return this->name;
}

string Method::getnamespace() {
	return this->ns;
}

string Method::getdisplayname() {
	if (this->ns.length() == 0) {
		return this->name;
	} else {
		return this->ns + "." + this->name;
	}
}
