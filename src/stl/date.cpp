#include "date.h"
#include "native.h"
#include "../headers/utils.h"

#ifdef _WIN32
#	include <windows.h>
#else
#	include <time.h>
#endif

variable* date_time::run(int paramc, variable* params[]) {
#ifdef _WIN32
	SYSTEMTIME st;
	GetSystemTime(&st);

	int t = st.wMilliseconds * 1000;
#else
	time_t t = time(NULL);
#endif

	return new str((long long) t);
}
