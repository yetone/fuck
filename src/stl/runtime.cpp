#include "runtime.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include "native.h"

variable* sleep::run(int paramc, variable* params[]) {
	int x = 1;

#ifdef _WIN32
	Sleep(x);
#else
	usleep(x * 1000);
#endif

	return nullptr;
}
