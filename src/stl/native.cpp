#include "native.h"

#include "runtime.h"

nativefuncs_t nativefuncs;

void addfunc(wstring name, templ *t) {
	nativefuncs.push_back(make_pair(name, t));
}

void loadstl() {
	addfunc(L"sleep", new sleep);
}
