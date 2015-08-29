#include "native.h"

/**
 * Pause code execution
 */
class sleep : public templ {
	variable* run(int paramc, variable* params[]);
	wstring getname();
	int getparamc();
};
