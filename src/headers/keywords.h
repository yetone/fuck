#include <string>

using namespace std;

#define COMMENT_1 L"#"
#define COMMENT_2 L"//"

#define PAIR_SEPARATOR "=>"

#define ENTRY_POINT L"main"

#define KEYWORD_SEPARATOR ' '

#define KW_OPEN "{"
#define KW_END "}"
#define KW_END_SECTION "end"

#define KW_FALSE "false"
#define KW_TRUE "true"

#define KW_NAMESPACE "namespace"

#define KW_METHOD "method"

#define KW_READ "read"
#define KW_PRINT "out"
#define KW_PRINT_ERR "err"

#define KW_CALL_METHOD "call"

#define KW_LABEL "label"
#define KW_GOTO "goto"

#define KW_UNSET_VAR "unset"
#define KW_SET_VAR "set"
#define KW_SET_FRONT /* set */ "front"
#define KW_SET_BACK /* set */ "back"

#define KW_ARRAY "array"

#define KW_PUSH_VAR_SIMPLE ":="
#define KW_PUSH_VAR_SIMPLE_KEY "push-simple"

#define KW_VAR_SIGN "$"
#define KW_VAR_SIGN_KEY "sign"

#define KW_IF "if"
#define KW_ELSEIF "elif"
#define KW_ELSE "else"

#define KW_WHILE "while"

#define KW_FOR "for"
#define KW_FOREACH "foreach"
#define KW_FOR_FROM "from"
#define KW_FOR_TO "to"
#define KW_FOR_DO "do"

#define KW_HALT "halt"

#define KW_BREAK "break"
#define KW_CONTINUE "continue"
#define KW_RETURN "return"

#define KW_EXPR "expr"

#define OP_EQUALS "=="
#define OP_NOT_EQ "!="
#define OP_LESS_THAN "<"
#define OP_MORE_THAN ">"
#define OP_LESS_OR_EQUALS "<="
#define OP_MORE_OR_EQUALS ">="

#define OP_CONCATENATE_STRINGS "."

#define KW_IN "in"
#define KW_NOT_IN "ni"

#define KW_INCR "incr"
#define KW_DECR "decr"

#define OP_AND "&&"
#define KW_AND "and"
#define OP_OR "||"
#define KW_OR "or"
#define OP_XOR "^^"
#define KW_XOR "xor"

#define KW_USE "use"
#define KW_IMPORT_FROM "from"
#define KW_IMPORT "import"

void load();

wstring get_kw(wstring key);
wstring get_kw(string key);
wstring get_kw(string key, string def);
wstring get_kw(string key, wstring def);
