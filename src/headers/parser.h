#include <string>
#include <map>

#include "code.h"
#include "method.h"

using namespace std;

typedef vector<Method*> methodmap;

// Check if line is comment
inline bool is_comment(string);

// Check if line is goto label
inline bool is_label(string);

void parse(Code*);

// Invoke main method
void invoke();

// Invoke method from string
void invoke(string);

// Invoke method
void invoke(Method*);

// Execute line
void execline(Method*, unsigned int*, int indent);

// Execute range of lines
void execrange(Method*, unsigned int*, unsigned int, int indent);

// check if something is true
bool check_cond(string);

void parseif(Method* method, string line, unsigned int* i, int);

struct If {
	int start;
	int end;
};
