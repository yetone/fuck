#include <string>

using namespace std;

template <typename T>
class Variable {
public:
	T var;
	string name;
};


typedef Variable<string> defvar;
typedef vector<defvar> stack;
