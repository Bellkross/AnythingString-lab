#ifndef SYMBOL_H_
#define SYMBOL_H_
#include <iostream>
using namespace std;

template <typename Format>
struct FormattedSymbol
{
	char _a;
	Format _f;
	operator int() const
	{
		return _a;
	}
};

template <typename Format>
void fillFormattedString(FormattedSymbol<Format>*& fs, char* a, Format* s)
{
	fs = new FormattedSymbol<Format>[strlen(a)+1];
	fs[strlen(a)]._a = 0;
	for (size_t i = 0; i < strlen(a); ++i)
	{ 
		fs[i]._a = a[i];
		fs[i]._f=s[i];
	}
	return;
}

template <typename T>
size_t length(const T* t)
 {
	 size_t n = 0;
	 while (*t++)
		 ++n;
	 return n;
 }

template <typename Format>
ostream& operator<<(ostream & os, const FormattedSymbol<Format>* fs)
{
	for(size_t i=0; i<length(fs); ++i)
		os<<'('<<fs[i]._a<<','<< fs[i]._f<<')';
	return os;
}

#endif // SYMBOL_H_