#include <string>

#include "code.h"

using namespace std;

// Verbose printing mode
enum class verbose_mode {
	NORMAL,
	DELETION,
	ADDITION,
	COMMENT
};

// Check if string starts with another string
bool startswith(string, string);
bool startswith(wstring, wstring);

// Check if string contains another string
bool contains(string s, string find);
bool contains(wstring s, wstring find);

// Trim string from whitespace characters
wstring trim(wstring);

// Print verbose message
void printverbose(wstring s, verbose_mode mode = verbose_mode::NORMAL);

// Print (fatal) error message
void printerror(string);
void printerror(wstring);

// Print warning message
void printwarning(string);
void printwarning(wstring);

/*
 * @param str String to replace in
 * @param from
 * @param to
 */
wstring replaceAll(wstring str, wstring from, wstring to);

/*
 * Split string to vector
 * @param str String to split
 * @param delim Delimiter
 */
vector<wstring> split(wstring str, wchar_t delim = ' ');

/*
 * Get exit code from string
 */
int get_exit_code(wstring);

// Integer to wide string
wstring itow(int);

// Integer to string
string itos(int);

// Wide string to integer
int wtoi(wstring);

// Wide string to double
double wtod(wstring);

// String to wide string
wstring stow(string);

// Wide string to string
string wtos(wstring);
