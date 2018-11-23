#ifndef _exceptions_h_
#define _exceptions_h_

#include <string>
#include <iostream>
using namespace std;

class Exception 
{
protected:
	string _message;
public:
	Exception(const string& message="Exception"): _message(message) {};
	virtual const string& msg() const { return _message; };
};

class BadStringException : public Exception
{
public:
	BadStringException(const string& message = "Bad string exception"): Exception(message) {};
};

class BadIndexException : public Exception
{
private:
	const size_t _index;
public:
	BadIndexException(const size_t index = 0, const string& message = "Bad index exception"): Exception(message), _index(index)
	{};
	const size_t index() const { return _index; };
};

class UndefinedPointerException : public Exception
{
public:
	UndefinedPointerException(const string& message = "Undefined pointer exception"): Exception(message) {};
};

class MemoryException : public Exception
{
public:
	MemoryException(const string& message = "Memory exception"): Exception(message) {};
};

ostream& operator<<(ostream& os, const Exception& e)
{
	return os << e.msg(); 
}
#endif