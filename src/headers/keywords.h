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

#define KW_GOTO_LABEL ":"
#define KW_GOTO_LABEL_KEY "goto-label"
#define KW_GOTO "goto"

#define KW_SET_VAR "set"
#define KW_GET_VAR "get"
#define KW_VAR_SIGN "$"
#define KW_VAR_SIGN_KEY = "sign"

#define KW_IF "if"
#define KW_ENDIF = "endif"

void load();

string get_kw(string key);
string get_kw(string key, string def);
