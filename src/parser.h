#include <string>
#include <map>

#include "code.h"

using namespace std;

typedef vector<Method*> methodmap;

inline bool iscomment(string);

void parse(Code*);

void invoke();

void invoke(string);

void invoke(Method*);
