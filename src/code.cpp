#include <fstream>
#include <vector>

#include "code.h"

using namespace std;

Code::Code(string file): file(file) {
	vector<string> lines;

	ifstream input(file.c_str());

	string line;
	while (getline(input, line)) {
		lines.push_back(line);
	}

	this->lines = lines;
}

Code::Code(string file, vector<string> lines): file(file), lines(lines) {

}

vector<string> Code::getlines() {
	return this->lines;
}

string Code::getfile() {
	return this->file;
}

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
