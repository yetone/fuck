#include "native.h"

/**
 * Pause code execution
 */
class sleep : public templ {
	virtual variable* run(int paramc, variable* params[]);
	virtual wstring getname();
};
