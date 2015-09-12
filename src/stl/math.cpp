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

variable* math_acos::run(int paramc, variable* params[]) {
	double x = wtod(params[0]->get());
	double t = acos(x);

	return new str(t);
}

variable* math_asin::run(int paramc, variable* params[]) {
	double x = wtod(params[0]->get());
	double t = asin(x);

	return new str(t);
}

variable* math_atan::run(int paramc, variable* params[]) {
	double x = wtod(params[0]->get());
	double t = atan(x);

	return new str(t);
}

variable* math_atan2::run(int paramc, variable* params[]) {
	double x = wtod(params[0]->get());
	double x2 = wtod(params[1]->get());

	double t = atan2(x, x2);

	return new str(t);
}
