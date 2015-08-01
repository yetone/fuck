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
	LESS_OR_EQUALS,

	// Check if single variable is true
	SINGLE,
};

// Check if line is comment
inline bool is_comment(wstring);

Variable* getvar(wstring);

Variable setvar(wstring name, wstring statement, StackPos pos = StackPos::END);

void parse(Code&);

// Invoke main method
void invoke();

// Invoke method from wstring
Variable* invoke(wstring);

// Invoke method
Variable* invoke(Method*);

// Execute line
ReturnType execline(Method*, unsigned int*, int indent, Variable*&);

// Execute range of lines
ReturnType execrange(Method*, unsigned int*, unsigned int, int indent, Variable*&);

// check if something is true
bool check_cond(wstring);

bool check_cond_compare(wstring cond);
bool check_cond_compare(const wstring& var1, const wstring& var2, Relational ret);

// used when set keyword is used, create new variable
wstring parse_set_statement(wstring statement);

ReturnType parseif(Method* method, wstring line, unsigned int* i, int, Variable*&);

ReturnType parsewhile(Method* method, wstring, unsigned int* i, int, Variable*&);

ReturnType parsefor(Method* method, wstring, unsigned int* i, int, Variable*&);

typedef Chunk If;
typedef Chunk While;
typedef Chunk For;
