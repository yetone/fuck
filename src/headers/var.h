#include <string>
#include <vector>

using namespace std;

#ifndef H_VAR
#define H_VAR

class Variable {
public:
	wstring var;
	wstring name;
};

#endif

typedef vector<Variable> stackmap;
