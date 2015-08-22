#include <string>
#include <vector>

#include "code.h"

using namespace std;

typedef pair<unsigned int, wstring> line;
typedef vector<line> linemap;
typedef vector<wstring> parameters;

#ifndef H_METHOD
#define H_METHOD

class Method {
public:
	struct Chunk chunk;
	const wstring ns;
	const wstring name;
	parameters params;
	linemap lines;
	goto_labels labels;

	Method(wstring name);
	Method(wstring ns, wstring name);
	Method(wstring ns, wstring name, parameters params);
	Method(wstring ns, wstring name, linemap& lines);
	linemap& getlines();
	const wstring& getname();
	const wstring& getnamespace();
	const wstring getdisplayname();

	const bool ismain();
};

#endif
