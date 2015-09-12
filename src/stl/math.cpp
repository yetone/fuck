#include "math.h"
#include "native.h"
#include "../headers/utils.h"

#include <cmath>

variable* math_cos::run(int paramc, variable* params[]) {
	double x = wtod(params[0]->get());
	double t = cos(x);

	return new str(t);
}

variable* math_sin::run(int paramc, variable* params[]) {
	double x = wtod(params[0]->get());
	double t = sin(x);

	return new str(t);
}

variable* math_tan::run(int paramc, variable* params[]) {
	double x = wtod(params[0]->get());
	double t = tan(x);

	return new str(t);
}
