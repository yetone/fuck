#include <string>

using namespace std;

#define KW_END "end_"

#define KW_FALSE "false"
#define KW_TRUE "true"

#define KW_NAMESPACE "namespace"

#define KW_METHOD "method"

#define KW_PRINT "print"

void load();

string get(string key);
string get(string key, string def);
