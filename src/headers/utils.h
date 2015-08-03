#include <string>

#include "code.h"

using namespace std;


enum class verbose_mode {
	NORMAL,
	DELETION,
	ADDITION,
	COMMENT
};

bool startswith(string, string);
bool contains(string s, string find);
bool startswith(wstring, wstring);
bool contains(wstring s, wstring find);

wstring trim(wstring);

void printverbose(wstring s, verbose_mode mode = verbose_mode::NORMAL);

void printerror(string);

void printwarning(string);

void printerror(wstring);

void printwarning(wstring);

wstring replaceAll(wstring str, wstring from, wstring to);

vector<wstring> split(wstring&, wchar_t delim = ' ');

int get_exit_code(wstring);

int wtoi(wstring);

double wtod(wstring);

wstring stow(string);

string wtos(wstring);
