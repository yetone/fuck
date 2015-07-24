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

#define KW_PUSH_VAR "push"
#define KW_PUSH_FRONT /* push */ "front"
#define KW_PUSH_BACK /* push */ "back"

#define KW_PUSH_VAR_SIMPLE ":="
#define KW_PUSH_VAR_SIMPLE_KEY "push-simple"

#define KW_VAR_SIGN "$"
#define KW_VAR_SIGN_KEY "sign"

#define KW_IF "if"
#define KW_ENDIF "endif"
#define KW_ELSEIF "elif"
#define KW_ELSE "else"

#define KW_WHILE "while"
#define KW_WHILE_END "end_while"

#define KW_FOR "for"
#define KW_FOR_END "end_for"
#define KW_FOR_FROM "from"
#define KW_FOR_TO "to"
#define KW_FOR_DO "do"

#define KW_HALT "halt"

#define KW_BREAK "break"
#define KW_CONTINUE "continue"
#define KW_RETURN "return"

void load();

string get_kw(string key);
string get_kw(string key, string def);
