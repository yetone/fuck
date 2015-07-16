#include <string>
#include <map>

#include "code.h"
#include "method.h"

using namespace std;

typedef vector<Method*> methodmap;

inline bool is_comment(string);

inline bool is_label(string);

void parse(Code*);

void invoke();

void invoke(string);

void invoke(Method*);

void execline(Method*, unsigned int*);
