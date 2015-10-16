#include "native.h"

class date_time : public templ {
public:
	variable* run(int paramc, variable* params[]);

	const wstring getname() {
		return L"time";
	}

	const int getparamc() {
		return 0;
	}
};
