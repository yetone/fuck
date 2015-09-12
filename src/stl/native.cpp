#include "native.h"

#include "runtime.h"
#include "math.h"

#include "../headers/utils.h"

nativefuncs_t nativefuncs;

extern methodmap methodMap;

void addfunc(wstring name, templ *t) {
	nativefuncs.push_back(make_pair(name, t));
}

void loadstl() {
	addfunc(L"runtime", new sleep);
	addfunc(L"math", new math_cos);
	addfunc(L"math", new math_sin);
	addfunc(L"math", new math_tan);
	addfunc(L"math", new math_acos);
	addfunc(L"math", new math_asin);
	addfunc(L"math", new math_atan);
	addfunc(L"math", new math_atan2);

}

void import_native(wstring name, wstring from) {
	for (nativefunc_t n : nativefuncs) {
		if ((from == EMPTY && n.first == name) || (from != EMPTY && n.first == from && n.second->getname() == name)) {
			NativeMethod* m = new NativeMethod(n.second->getname());
			methodMap.push_back(m);
		}
	}

}

variable* call_native(wstring name, int paramc, variable* params[]) {
	for (Method* m : methodMap) {
		if (m->getname() == name && m->isnative()) {
			for (nativefunc_t n : nativefuncs) {
				templ* t = n.second;

				if (t->getname() == name) {
					if (t->getparamc() != paramc) {
						throw runtime_error("Not matching parameter count " + itos(paramc) + " (should be " + itos(t->getparamc()) + ")");
					}

					variable* v = t->run(paramc, params);

					return v;
				}
			}
		}
	}

	return nullptr;
}
