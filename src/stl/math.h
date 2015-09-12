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


class math_acos : public templ {
public:
	variable* run(int paramc, variable* params[]);

	const wstring getname() {
		return L"acos";
	}

	const int getparamc() {
		return 1;
	}
};


class math_asin : public templ {
public:
	variable* run(int paramc, variable* params[]);

	const wstring getname() {
		return L"asin";
	}

	const int getparamc() {
		return 1;
	}
};


class math_atan : public templ {
public:
	variable* run(int paramc, variable* params[]);

	const wstring getname() {
		return L"atan";
	}

	const int getparamc() {
		return 1;
	}
};


class math_atan2 : public templ {
public:
	variable* run(int paramc, variable* params[]);

	const wstring getname() {
		return L"atan2";
	}

	const int getparamc() {
		return 2;
	}
};
