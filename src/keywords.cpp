#include <string>

#include "headers/keywords.h"

using namespace std;

void load() {

}

string get_kw(string key) {
	return get_kw(key, key);
}

string get_kw(string key, string def) {
	return def;
}
