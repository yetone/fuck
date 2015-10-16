#include <string>
#include <vector>
#include <map>
#include <memory>

#include "utils.h"

using namespace std;

typedef map<wstring, wstring> array_t;

#ifndef H_VAR
#define H_VAR

// Temporary variable name
#define EMPTY L""

// Variable type
enum class type {
	// Text, number, decimal
	DEFAULT,

	// Array/map
	ARRAY,

	// Pointer variable
	POINTER
};

class variable {
private:
	// Variable name (empty if temporary)
	wstring name;

public:
	variable();
	variable(wstring name);

	virtual ~variable() { }

	virtual wstring get(const wstring& key) {
		return EMPTY;
	}

	virtual wstring get() {
		return EMPTY;
	}

	virtual void set(const wstring& value) { }

	virtual type gettype() {
		return type::DEFAULT;
	}

	wstring getname();
};

// Default variable
class str : public variable {
private:
	wstring var;

public:
	str(): variable() { }

	str(wstring name): variable(name) { }

	str(wstring name, wstring value): variable(name) {
		this->var = value;
	}

	str(wstring name, double value): variable(name) {
		this->var = dtow(value);
	}

	str(wstring name, int value): variable(name) {
		this->var = itow(value);
	}

	str(double value) : variable() {
		this->var = dtow(value);
	}

	str(int value) : variable() {
		this->var = itow(value);
	}

	str(long value) : variable() {
		this->var = ltow(value);
	}

	wstring get();

	void set(const wstring& value);
};

// Array variable
class arrays : public variable {
private:
	array_t var;

public:
	arrays(): variable() {

	}

	arrays(wstring name): variable(name) {

	}

	wstring get(const wstring& key);
	wstring get();
	array_t* getpairs();

	void set(const wstring& key, const wstring& value);

	type gettype() {
		return type::ARRAY;
	}
};

class pointer : public variable {
private:
	void* ptr;

public:
	pointer(): variable() {

	}

	pointer(wstring name): variable(name) {

	}

	void* getptr() {
		return ptr;
	}

	void setptr(void* ptr) {
		this->ptr = ptr;
	}

	wstring get();

	type gettype() {
		return type::POINTER;
	}
};

#endif

// Return display text from variable
wstring gettext(variable*);

typedef vector<variable*> stackmap;
