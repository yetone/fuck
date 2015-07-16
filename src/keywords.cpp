#include <string>

#include "headers/keywords.h"

using namespace std;

void load() {

}

string get(string key) {
	return get(key, key);
}

string get(string key, string def) {
	return def;
}
