#include <string>
#include <vector>

using namespace std;

#ifndef H_VAR
#define H_VAR

template <typename T>
class Variable {
public:
	T var;
	string name;
};

#endif

typedef Variable<string> defvar;
typedef vector<defvar> stack;
