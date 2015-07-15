#include <string>
#include <map>

#include "code.h"

using namespace std;

typedef vector<Method*> methodmap;

void parse(Code*);

void invoke();

void invoke(string);

void invoke(Method*);
