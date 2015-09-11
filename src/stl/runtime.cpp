#include "runtime.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include "native.h"

#include "../headers/utils.h"

variable* sleep::run(int paramc, variable* params[]) {
	int x = wtoi(params[0]->get());

#ifdef _WIN32
	Sleep(x);
#else
	// Microseconds on *nix, ms 10^3
	usleep(x * 1000);
#endif

	return nullptr;
}
