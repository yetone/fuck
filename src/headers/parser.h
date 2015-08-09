#include <vector>

#include "method.h"

using namespace std;

vector<Chunk> parse_chunks(Method* method, unsigned int* i, int* totalend, vector<wstring> keywords);

wstring parsevars(wstring);
