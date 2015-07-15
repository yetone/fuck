#include <string>

using namespace std;

#define TRIM "\t\r\n "

extern bool verbose;

bool startswith(string, string);

string trim(string);

void printverbose(string);

void err(string);

string parsevars(string);
