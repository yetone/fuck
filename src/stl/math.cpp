#include "math.h"
#include "native.h"
#include "../headers/utils.h"

#include <cmath>

variable* math_cos::run(int paramc, variable* params[]) {
	double x = wtod(params[0]->get());

	double t = cos(x);

	variable* v = new str;
	v->set(dtow(t));

	return nullptr;
}

variable* math_sin::run(int paramc, variable* params[]) {
	double x = wtod(params[0]->get());

	double t = sin(x);

	variable* v = new str;
	v->set(dtow(t));

	return nullptr;
}

variable* math_tan::run(int paramc, variable* params[]) {
	double x = wtod(params[0]->get());

	double t = tan(x);

	variable* v = new str;
	v->set(dtow(t));

	return nullptr;
}
