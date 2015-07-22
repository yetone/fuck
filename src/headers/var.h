#include <string>
#include <vector>

using namespace std;

#ifndef H_VAR
#define H_VAR

class Variable {
public:
	string var;
	string name;
};

#endif

typedef vector<Variable> stack;
