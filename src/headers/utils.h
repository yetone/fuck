#include <string>

#include "code.h"

using namespace std;

#define TRIM "\t\r\n "

bool startswith(string, string);

string trim(string);

void printverbose(string, bool star = true);

void printerror(string);

void printwarning(string);

string parsevars(string);

string replaceAll(string str, string from, string to);

vector<string> split(string&, char delim = ' ');

int get_exit_code(string);
