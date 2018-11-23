#ifndef _AnythingString_h_
#define _AnythingString_h_
#include <iostream>
#include <cassert>
#include <string>
#include "exceptions.h"
using namespace std;

template <class T>
class AnythingString
{
	friend class Tester;
private:
	struct StringRep;
	StringRep* _rep;
	//without realization
	AnythingString(const T*);
	AnythingString& operator=(const T*);
	AnythingString();
public:
	class Proxy;

	AnythingString(const T*, const size_t);
	AnythingString(const AnythingString<T>& as);
	~AnythingString();
	AnythingString& operator=(const AnythingString<T>& as);
	void check(size_t i) const;
	const T& read(int i) const;
	void write (int i, T c);

	Proxy operator[](int i);
	const T& operator[](int i) const;

	size_t length() const { return _rep->_len; };
};

template <class T>
struct AnythingString<T>::StringRep
{
//	pointer to the allocation of a string
	T* _allocator;
//	number of characters in the string
	size_t _len;		
//	how many oblects use this representation
	int _refCounter;
//	if the string may be sharable
	bool _shareable;

	StringRep (size_t len = 0, const T* allocator = 0);
	~StringRep();
//	pseudo copy constructor
	StringRep* getOwnCopy();
//	pseudo assignment
	void assign (size_t, const T*);
	void makeShareable() {_shareable=true;}
	void makeUnshareable() {_shareable=false;}
	bool isShareable() const { return _shareable;}
private:
//	Wil never be implemented
	StringRep (const StringRep&);
	StringRep& operator= (const StringRep&);
};

template <class T>
class AnythingString<T>::Proxy
{
	friend class AnythingString<T>;
	// template operator for any streams
	template <class OStream>
	friend OStream& operator<<(OStream& os, const Proxy& charp)
	{
		os << static_cast<T>(charp);
		return os;
	};
private:
//	String to which belongs proxy
	AnythingString<T> & _proxyship;
//	Character substituted by a proxy
	int _index;
	Proxy(AnythingString<T>& as, int i);

public:
	const T* operator&() const
	{
		return &_proxyship._rep->_allocator[_index];
	};
	T* operator&()
	{
		if (_proxyship._rep->isShareable() && _proxyship._rep->_refCounter>1)
		{
			_proxyship._rep->_refCounter--;
			_proxyship._rep = new StringRep(_proxyship._rep->_len, 
												_proxyship._rep->_allocator);
		}
		_proxyship._rep->makeUnshareable();
		return &_proxyship._rep->_allocator[_index];
	};
	operator const T&() const
	{
		return _proxyship.read(_index);
	};
	operator T&()
	{
		if (_proxyship._rep->isShareable() && _proxyship._rep->_refCounter>1)
		{
			_proxyship._rep->_refCounter--;
			_proxyship._rep = new StringRep(_proxyship._rep->_len, 
												_proxyship._rep->_allocator);
		}
		_proxyship._rep->makeUnshareable();
		return _proxyship._rep->_allocator[_index];
	}
	Proxy& operator=(T c)
	{
		_proxyship.write(_index,c);
		return *this;
	}
};

/* Anything string implementation */

template <class T>
AnythingString<T>::AnythingString(const T* allocator, const size_t len): _rep(new StringRep(len, allocator))
{};

template <class T>
AnythingString<T>::AnythingString(const AnythingString<T>& as)
{
	if (as._rep->isShareable())
//	this representation will be shared
//	with those of ds
	{
		as._rep->_refCounter++;
		_rep=as._rep;
	}
	else
//	create a new representation
		_rep=new StringRep(as._rep->_len,as._rep->_allocator);
}

template <class T>
AnythingString<T>::~AnythingString()
{
	if(--_rep->_refCounter==0)
		delete _rep;
};

template <class T>
AnythingString<T>& AnythingString<T>::operator=(const AnythingString<T>& as)
{
//	protection from self assignment
	as._rep->_refCounter++;
	assert ((&as==this)?_rep->_refCounter>1:true);

	if (--_rep->_refCounter==0)
//	never occurs in the case of self assignment
		delete _rep;
	if (as._rep->isShareable())
//	add this to a shared representation of as
		_rep=as._rep;
	else
//	create a new representation
		_rep=new StringRep(as._rep->_len,as._rep->_allocator);
	return *this;
};

template <class T>
void AnythingString<T>::check(size_t i) const
{
	// check if the index is within of the range
	if (_rep->_len<=i)
		throw BadIndexException(i);
};

template <class T>
const T& AnythingString<T>::read(int i) const
{
	return _rep->_allocator[i];
}

template <class T>
void AnythingString<T>::write(int i, T c)
{
	// Construct a copy existing representation
	_rep = _rep->getOwnCopy();
	// Substitute the i-th character in it
	_rep->_allocator[i]=c;
}

template <class T>
typename AnythingString<T>::Proxy AnythingString<T>::operator[](int i)
{
	check(i);
//	A proxy will be returned instead of a T
	return Proxy(*this,i);
}

template <class T>
const T& AnythingString<T>::operator[](int i) const
{
	check(i);
//	A character itself may be returned
	return _rep->_allocator[i];
}

/* StringRep implementation */

template <class T>
AnythingString<T>::StringRep::StringRep(size_t len, const T* allocator):
	_shareable (true),
	_refCounter (1),
	_len(len),
	_allocator(new (nothrow) T[len])
{
	if(allocator==0)
		throw UndefinedPointerException ("Attempt to use not defined pointer");
	if (_allocator == 0)
		throw MemoryException("No more space in the system");
	for(size_t i = 0; i < len; ++i)
		_allocator[i] = allocator[i];
};

template <class T>
AnythingString<T>::StringRep::~StringRep()
{
	delete [] _allocator;
};

template <class T>
typename AnythingString<T>::StringRep* AnythingString<T>::StringRep::getOwnCopy()
{
	if(_refCounter==1)
		return this;
	_refCounter--;
	return new StringRep(_len,_allocator);
};

template <class T>
void AnythingString<T>::StringRep::assign(size_t len, const T* allocator)
{
	if(allocator==0)
		throw UndefinedPointerException ("Attempt to use not defined pointer");
	if(_len!=len)
	{
		delete[] _allocator;
		_len = len;
		_allocator = new (nothrow) char[_len+1];
		if (_allocator == 0)
			throw MemoryException("No more space in the system");
	}
	memcpy(_allocator, allocator, _len);
};

/* T proxy implementation */

template <class T>
AnythingString<T>::Proxy::Proxy(AnythingString<T>& as, int i): _proxyship(as), _index(i)
{};

/* Utills */

// template operator for any streams
template <class OStream, class T>
OStream& operator<<(OStream& os, const AnythingString<T>& as)
{
	for(size_t i=0; i < as.length(); i++)
		os<<static_cast<T>(as[i]);
	return os;
};

#endif