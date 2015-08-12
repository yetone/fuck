#include <vector>

#include "method.h"

using namespace std;

void parse(Code&);

vector<Chunk> parse_chunks(Method* method, unsigned int* i, int* totalend, wstring open, vector<wstring> *separators = nullptr);

wstring parsevars(wstring, stackmap&);
