#include <string>

using namespace std;

#define FORMAT_BOLD "\e[1m"
#define FORMAT_DIM "\e[2m"
#define FORMAT_UNDERLINE "\e[4m"
#define FORMAT_BLINK "\e[5m"
#define FORMAT_INVERTED "\e[7md"
#define FORMAT_HIDDEN "\e[8m"

#define COLOR_RESET "\e[39m"
#define COLOR_BLACK "\e[30m"
#define COLOR_RED "\e[31m"
#define COLOR_GREEN "\e[32m"
#define COLOR_YELLOW "\e[33m"
#define COLOR_BLUE "\e[34m"
#define COLOR_MAGENTA "\e[35m"
#define COLOR_CYAN "\e[36m"
#define COLOR_LIGHT_GRAY "\e[37m"
#define COLOR_DARK_GRAY "\e[90m"
#define COLOR_LIGHT_RED "\e[91m"
#define COLOR_LIGHT_GREEN "\e[92m"
#define COLOR_LIGHT_YELLOW "\e[93m"
#define COLOR_LIGHT_BLUE "\e[94m"
#define COLOR_LIGHT_MAGENTA "\e[95m"
#define COLOR_LIGHT_CYAN "\e[96m"
#define COLOR_WHITE "\e[97m"

#define VERBOSE COLOR_YELLOW
#define VERBOSE_HL COLOR_LIGHT_YELLOW

#define ERROR COLOR_RED
#define ERROR_HL COLOR_LIGHT_RED

string reset();
string color(string);
