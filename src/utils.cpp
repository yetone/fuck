#include <string>

using namespace std;

bool startswith(string s, string s1) {
	return s.substr(0, s1.size()) == s1;
}
