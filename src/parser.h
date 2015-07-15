#include <string>
#include <map>

#include "code.h"

using namespace std;

typedef vector<Method*> methodmap;

methodmap* parse(Code*);

void run(methodmap*);

void run(Method* m);
