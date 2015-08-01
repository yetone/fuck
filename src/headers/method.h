#include <string>
#include <vector>

#include "code.h"

using namespace std;

#ifndef H_METHOD
#define H_METHOD

class Method {
public:
	struct Chunk chunk;
	const wstring ns;
	const wstring name;
	vector<wstring> lines;
	goto_labels labels;

	Method(wstring name);
	Method(wstring ns, wstring name);
	Method(wstring ns, wstring name, vector<wstring>& lines);
	vector<wstring>& getlines();
	const wstring& getname();
	const wstring& getnamespace();
	const wstring getdisplayname();
};

#endif
