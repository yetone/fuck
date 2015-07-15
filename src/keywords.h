#include <string>

using namespace std;

#define COMMENT_1 "#"
#define COMMENT_2 "//"

#define ENTRY_POINT "main"

#define KEYWORD_SEPARATOR ' '

#define KW_END "end_"

#define KW_FALSE "false"
#define KW_TRUE "true"

#define KW_NAMESPACE "namespace"

#define KW_METHOD "method"

#define KW_PRINT "out"
#define KW_PRINT_ERR "err"

#define KW_CALL_METHOD "call"

void load();

string get(string key);
string get(string key, string def);
