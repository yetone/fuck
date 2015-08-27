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
private:
	const wstring ns;
	const wstring name;
	parameters params;

public:
	struct Chunk chunk;
	linemap lines;
	goto_labels labels;

	virtual ~Method() { }
	Method(wstring name);
	Method(wstring ns, wstring name);
	Method(wstring ns, wstring name, parameters params);
	Method(wstring ns, wstring name, linemap& lines);

	linemap& getlines();
	parameters& getparams();

	const wstring& getname();
	const wstring& getnamespace();
	const wstring getdisplayname();

	const bool ismain();

	virtual const bool isnative() {
		return false;
	}
};

class NativeMethod : public Method {
public:
	NativeMethod(wstring name): Method(name) { }

	const bool isnative() {
		return true;
	}
};

#endif
