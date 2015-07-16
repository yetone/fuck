#include <string>

#include "code.h"

using namespace std;

#define TRIM "\t\r\n "

extern bool verbose;

bool startswith(string, string);

string trim(string);

void printverbose(string);

void err(string);

string parsevars(string);

string replaceAll(string str, string from, string to);
