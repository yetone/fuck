#include <string>
#include <vector>
#include <map>
#include <memory>

using namespace std;

typedef map<wstring, wstring> array_t;

#ifndef H_VAR
#define H_VAR

enum class type {
	DEFAULT,
	ARRAY
};

class variable {
public:
	virtual ~variable() { }
	wstring name;

	virtual wstring get(const wstring& key) {
		return L"";
	}
	virtual wstring get() {
		return L"";
	}

	virtual void set(const wstring& key, const wstring& value) { }
	virtual void set(const wstring& value) { }
};

class str : public variable {
public:
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
	array_t var;

	wstring get(const wstring& key);
	wstring get();

	void set(const wstring& key, const wstring& value);
	void set(const wstring& value);
};

#endif

typedef vector<variable*> stackmap;
