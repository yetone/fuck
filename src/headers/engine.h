#include <string>
#include <map>

#include "var.h"
#include "code.h"
#include "method.h"

using namespace std;

typedef vector<Method*> methodmap;

// Expression types
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

// Modes when comparing multiple booleans
enum class Conds {
	AND,
	OR,
	XOR
};

// Modes when comparing two variables
enum class bool_ops {
	EQUALS,
	NOT_EQUAL,
	MORE_THAN,
	LESS_THAN,
	MORE_OR_EQUALS,
	LESS_OR_EQUALS,
	IN_ARRAY,
	NOT_IN_ARRAY,

	// Check if single variable is true
	SINGLE,
};

// Check if line is comment
inline bool is_comment(wstring);

variable* getvar(wstring, stackmap&);

arrays* setarr(wstring name, wstring statement, stackmap&);
variable* setvar(wstring statement, stackmap& stack, type t = type::DEFAULT);
variable* setvar(wstring name, wstring statement, stackmap& stack, type t = type::DEFAULT);
variable* setvar(wstring name, vector<wstring> statements, stackmap& stack, type t);

// Invoke main method
void invoke();

// Invoke method from wstring
variable* invoke(wstring);

// Invoke method
variable* invoke(Method*);

// Execute line
ReturnType execline(Method*, unsigned int*, variable*&, stackmap&);

// Execute range of lines
ReturnType execrange(Method*, unsigned int*, unsigned int, variable*&, stackmap&);

// check if something is true
bool check_cond(wstring, stackmap&);

bool check_cond_compare(wstring cond, stackmap&);
bool check_cond_compare(const wstring& var1, const wstring& var2, bool_ops ret, stackmap&);

// used when set keyword is used, create new variable
wstring parse_set_statement(wstring statement, stackmap&);

ReturnType parseif(Method* method, wstring line, unsigned int* i, variable*&, stackmap&);

ReturnType parsewhile(Method* method, wstring, unsigned int* i, variable*&, stackmap&);

ReturnType parsefor(Method* method, wstring, unsigned int* i, variable*&, stackmap&, bool range);

bool is_bool_expr(const wstring&);
bool is_math_expr(const wstring&);
bool is_array_expr(const wstring&);
bool is_string_expr(const wstring&);

// Unset variable from name
inline void unset(wstring, stackmap&);

// Unset all variables in array
void unset(int count, variable*[], stackmap&);

// Unset variable
void unset(variable*, stackmap&);
