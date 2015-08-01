#include <vector>

#include "method.h"

using namespace std;

vector<Chunk> parse_chunks(Method* method, unsigned int* i, int indent, int* totalend, vector<string> keywords);
