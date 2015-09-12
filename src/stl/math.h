#include "native.h"

/**
 * Returns the cosine of an angle of x radians.
 */
class math_cos : public templ {
public:
	variable* run(int paramc, variable* params[]);

	const wstring getname() {
		return L"cos";
	}

	const int getparamc() {
		return 1;
	}
};

/**
 * Returns the sine of an angle of x radians.
 */
class math_sin : public templ {
public:
	variable* run(int paramc, variable* params[]);

	const wstring getname() {
		return L"sin";
	}

	const int getparamc() {
		return 1;
	}
};

/**
 * Returns the tangent of an angle of x radians.
 */
class math_tan : public templ {
public:
	variable* run(int paramc, variable* params[]);

	const wstring getname() {
		return L"tan";
	}

	const int getparamc() {
		return 1;
	}
};
