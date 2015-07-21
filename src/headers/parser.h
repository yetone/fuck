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

// Check if line is comment
inline bool is_comment(string);

// Check if line is goto label
inline bool is_label(string);

defvar setvar(string name, string statement);

void parse(Code&);

// Invoke main method
void invoke();

// Invoke method from string
defvar* invoke(string);

// Invoke method
defvar* invoke(Method*);

// Execute line
ReturnType execline(Method*, unsigned int*, int indent, defvar*);

// Execute range of lines
ReturnType execrange(Method*, unsigned int*, unsigned int, int indent, defvar*);

// check if something is true
bool check_cond(string);

// used when set keyword is used, create new variable
string parse_set_statement(string statement);

ReturnType parseif(Method* method, string line, unsigned int* i, int, defvar*);

ReturnType parsewhile(Method* method, string, unsigned int* i, int, defvar*);

typedef Chunk If;
typedef Chunk While;
