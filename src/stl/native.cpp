#include "native.h"

#include "runtime.h"

nativefuncs_t nativefuncs;

void addfunc(wstring name, templ *t) {
	nativefuncs.push_back(make_pair(name, t));
}

void loadstl() {
	addfunc(L"sleep", new sleep);
}

void call_native(wstring name, int paramc, variable* params[]) {
	for (nativefunc_t func : nativefuncs) {
		if (func.first == name) {
			templ* t = func.second;

			t->run(paramc, params);
		}
	}
}
