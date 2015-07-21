#include <string>

#include "code.h"

using namespace std;

#define TRIM "\t\r\n "

bool startswith(string, string);

string trim(string);

void printverbose(string);

void printerror(string);

void printwarning(string);

string parsevars(string);

string replaceAll(string str, string from, string to);

int get_exit_code(string);
