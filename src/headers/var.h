#include <string>
#include <vector>

using namespace std;

#ifndef H_VAR
#define H_VAR

class variable {
public:
	virtual ~variable() { }

	wstring var;
	wstring name;

	virtual wstring& getraw();

	virtual wstring at(const int&);
	virtual wstring at(const wstring&);

	wstring operator [](const int& i) {
		return at(i);
	}
	wstring operator [](const wstring& w) {
		return at(w);
	}
};

class array : public variable {

	wstring& getraw();

	wstring at(const int&);
	wstring at(const wstring&);
};

#endif

typedef vector<variable> stackmap;
