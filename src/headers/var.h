#include <string>
#include <vector>
#include <map>
#include <memory>

using namespace std;

typedef map<wstring, wstring> array_t;

#ifndef H_VAR
#define H_VAR

#define EMPTY L""

enum class type {
	DEFAULT,
	ARRAY
};

class variable {
private:
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
	virtual array_t* getpairs() {
		return nullptr;
	}

	virtual void set(const wstring& key, const wstring& value) { }
	virtual void set(const wstring& value) { }

	virtual type gettype() { return type::DEFAULT; }

	wstring getname();
};

class str : public variable {
public:
	str(): variable() { }
	str(wstring name): variable(name) { }

	wstring var;

	wstring get(const wstring& key) {
		return get();
	}

	virtual wstring get();

	void set(const wstring& key, const wstring& value) {
		set(value);
	}
	void set(const wstring& value);
};

class arrays : public variable {
public:
	arrays(): variable() { }
	arrays(wstring name): variable(name) { }

	array_t var;

	wstring get(const wstring& key);
	wstring get();
	array_t* getpairs();

	void set(const wstring& key, const wstring& value);
	void set(const wstring& value);

	type gettype();
};

#endif

typedef vector<variable*> stackmap;
