#include "native.h"

/**
 * Pause code execution
 */
class sleep : public templ {
public:
	variable* run(int paramc, variable* params[]);

	const wstring getname() {
		return L"sleep";
	}

	const int getparamc() {
		return 1;
	}
};
