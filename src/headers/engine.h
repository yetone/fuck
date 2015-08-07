#include <string>
#include <map>

#include "var.h"
#include "code.h"
#include "method.h"

using namespace std;

typedef vector<Method*> methodmap;

enum class ExprType {
	BOOLEAN,
	MATH,
	NONE
};

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

variable* getvar(wstring);

variable* setvar(wstring name, wstring statement, type t = type::DEFAULT);
variable* setvar(wstring name, vector<wstring> statements, type t);

void parse(Code&);

// Invoke main method
void invoke();

// Invoke method from wstring
variable* invoke(wstring);

// Invoke method
variable* invoke(Method*);

// Execute line
ReturnType execline(Method*, unsigned int*, int indent, variable*&, stackmap&);

// Execute range of lines
ReturnType execrange(Method*, unsigned int*, unsigned int, int indent, variable*&, stackmap&);

// check if something is true
bool check_cond(wstring);

bool check_cond_compare(wstring cond);
bool check_cond_compare(const wstring& var1, const wstring& var2, Relational ret);

// used when set keyword is used, create new variable
wstring parse_set_statement(wstring statement);

ReturnType parseif(Method* method, wstring line, unsigned int* i, int, variable*&, stackmap&);

ReturnType parsewhile(Method* method, wstring, unsigned int* i, int, variable*&, stackmap&);

ReturnType parsefor(Method* method, wstring, unsigned int* i, int, variable*&, stackmap&);

bool is_bool_expr(wstring&);
bool is_math_expr(wstring&);

void unset(wstring);
void unset(variable*);
