#include <vector>

#include "method.h"

using namespace std;

/*
 * @param code
 * @param what If importing only one thing from the referenced code
 */
void parse(Code& code, wstring what = L"");

/*
 * Parse chunks in statements
 *
 * @param method Parent method
 * @param i Line number
 * @param totalend End of all chunks
 * @param open Open keyword
 * @param separators New chunk keywords
 */
vector<Chunk> parse_chunks(Method* method, unsigned int* i, int* totalend, wstring open, vector<wstring> *separators = nullptr);

// Replace variables in string
wstring parsevars(wstring, stackmap&);

wstring rm_strings(wstring);
