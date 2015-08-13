#include <map>
#include <stack>
#include <string>
#include <queue>

using namespace std;

struct TokenBase {
	// Needs to here to be able to compile
	virtual ~TokenBase() { };
};

template<class T> class Token : public TokenBase {
public:
	Token (T t) : val(t) {}
	T val;
};

typedef std::queue<TokenBase*> TokenQueue_t;

double calculate(const char* expr, std::map<std::string, double>* vars = 0);
TokenQueue_t toRPN(const char* expr, std::map<std::string, double>* vars, std::map<std::string, int> opPrecedence);

double eval(string);
