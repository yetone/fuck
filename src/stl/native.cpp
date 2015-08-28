#include "native.h"

#include "runtime.h"

#include <iostream>

nativefuncs_t nativefuncs;

extern methodmap methodMap;

void addfunc(wstring name, templ *t) {
	nativefuncs.push_back(make_pair(name, t));
}

void loadstl() {
	addfunc(L"runtime", new sleep);
}

void import_native(wstring name, wstring from) {
	for (nativefunc_t n : nativefuncs) {
		if ((from == EMPTY && n.first == from) || (from != EMPTY && n.first == from && n.second->getname() == name)) {
			NativeMethod* m = new NativeMethod(n.second->getname());
			methodMap.push_back(m);
		}
	}

}

void call_native(wstring name, int paramc, variable* params[]) {
	for (Method* m : methodMap) {
		if (m->getname() == name && m->isnative()) {
			for (nativefunc_t n : nativefuncs) {
				templ* t = n.second;

				if (t->getname() == name) {
					t->run(paramc, params);
					break;
				}
			}
		}
	}
}
