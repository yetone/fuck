#include <string>
#include <map>

#include "var.h"
#include "code.h"
#include "method.h"

using namespace std;

typedef vector<Method*> methodmap;

enum class ReturnType {
	CONTINUE,
	BREAK,
	RETURN,
	NONE
};

enum class StackPos {
	FRONT,
	END
};

enum class Conds {
	AND,
	OR,
	XOR
};

enum class Relational {
	EQUALS,
	NOT_EQUAL,
	MORE_THAN,
	LESS_THAN,
	MORE_OR_EQUALS,
	LESS_OR_EQUALS
};

// Check if line is comment
inline bool is_comment(string);

// Check if line is goto label
inline bool is_label(string);

Variable* getvar(string);

Variable setvar(string name, string statement, StackPos pos = StackPos::END);

void parse(Code&);

// Invoke main method
void invoke();

// Invoke method from string
Variable* invoke(string);

// Invoke method
Variable* invoke(Method*);

// Execute line
ReturnType execline(Method*, unsigned int*, int indent, Variable*&);

// Execute range of lines
ReturnType execrange(Method*, unsigned int*, unsigned int, int indent, Variable*&);

// check if something is true
bool check_cond(string);

bool check_cond_compare(string cond);
bool check_cond_compare(const string& var1, const string& var2, Relational ret);

// used when set keyword is used, create new variable
string parse_set_statement(string statement);

ReturnType parseif(Method* method, string line, unsigned int* i, int, Variable*&);

ReturnType parsewhile(Method* method, string, unsigned int* i, int, Variable*&);

ReturnType parsefor(Method* method, string, unsigned int* i, int, Variable*&);

typedef Chunk If;
typedef Chunk While;
typedef Chunk For;
