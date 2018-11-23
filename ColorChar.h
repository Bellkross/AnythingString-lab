#ifndef _ColorChar_h_
#define _ColorChar_h_
#include <iostream>
#include <Windows.h>
using namespace std;

enum Color
{
	Black         = 0,
	Blue          = 1,
	Green         = 2,
	Cyan          = 3,
	Red           = 4,
	Magenta       = 5,
	Brown         = 6,
	LightGray     = 7,
	DarkGray      = 8,
	LightBlue     = 9,
	LightGreen    = 10,
	LightCyan     = 11,
	LightRed      = 12,
	LightMagenta  = 13,
	Yellow        = 14,
	White         = 15
};

class ColorChar {
public:
	ColorChar(char c='?', Color color=LightGray): _c(c), _color(color) {};
	char c() const { return _c; };
	char& c() { return _c; };
	const Color& color() const { return _color; };
	Color& color() { return _color; };
private:
	char _c;
	Color _color;
};

void setColor(Color text = LightGreen, Color background=Black)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),(WORD)((background << 4) | text));
};

ostream& operator<<(ostream& os, const ColorChar& cc)
{
	setColor(cc.color());
	os << cc.c();
	setColor();
	return os;
};

#endif