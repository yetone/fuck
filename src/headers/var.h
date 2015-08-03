#include <string>
#include <vector>
#include <map>

using namespace std;

typedef map<wstring, wstring> array_t;

#ifndef H_VAR
#define H_VAR

class variable {
public:
	wstring name;
	wstring var;
};

class array {
public:
	wstring name;
	array_t arr;
};

#endif

typedef vector<variable> stackmap;
