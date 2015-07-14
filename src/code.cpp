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
	return this->ns + "." + this->name;
}
