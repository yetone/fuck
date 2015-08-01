#include <string>

#include "headers/keywords.h"

using namespace std;

void load() {

}

wstring get_kw(wstring key) {
	string s(key.begin(), key.end());
	return get_kw(s, s);
}

wstring get_kw(string key) {
	return get_kw(key, key);
}

wstring get_kw(string key, string def) {
	return wstring(def.begin(), def.end());
}

wstring get_kw(string key, wstring def) {
	return def;
}
