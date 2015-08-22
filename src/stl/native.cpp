#include "native.h"

#include "strings.h"

nativefuncs_t nativefuncs;

void addfunc(wstring name, templ *t) {
	nativefuncs.push_back(make_pair(name, t));
}

void loadstl() {
	addfunc(L"sub", new sub);
}
