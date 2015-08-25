#ifndef H_NATIVE

#define H_NATIVE

#include <string>
#include <utility>
#include <vector>

#include "../headers/var.h"

using namespace std;

class templ {
public:
	virtual ~templ() { }

	virtual variable* run(int paramc, variable* params[]) {
		return nullptr;
	}

};

typedef pair<wstring, templ*> nativefunc_t;
typedef vector<nativefunc_t> nativefuncs_t;

void addfunc(wstring name, templ *t);
void loadstl();

void call(wstring name);

#endif
