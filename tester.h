#ifndef _tester_h_
#define _tester_h_
#include "AnythingString.h"
#include "ColorChar.h"
#include "Symbol.h"
#include <iostream>
#include <cassert>
using namespace std;


class Tester {
public:
	Tester(){};
	~Tester(){};

	void run() 
	{
		stringRepTest();
		assignmentTests();
		accessTests();
		undefinedPointerTest();
		ñonstCornerCaseTest();
		nonConstCornerCaseTest();
	};
private:
	void stringRepTest()
	{
		cout << "0. String Rep Test " << endl;
		char* str = "Anything string of chars";
		char* secondStr = "Onything dashgh of hcars";
		char* thirdStr = "dashgh of hcars";
		int len = strlen(str);
		int len2 = strlen(thirdStr);
		AnythingString<char> as(str,len);
#ifndef NDEBUG
		tester(as);
#endif
		assert(as._rep->isShareable());
		assert(as._rep->_refCounter==1);
		assert(as._rep->_len==len);
		assert(assignmentTest(as));
		assert(accessTest(as));

		AnythingString<char>::StringRep* irep = as._rep->getOwnCopy();
		as._rep->assign(len, secondStr);
#ifndef NDEBUG
		tester(as);
#endif
		assert(as._rep->isShareable());
		assert(as._rep->_refCounter==1);
		assert(as._rep->_len==len);
		assert(assignmentTest(as));
		assert(accessTest(as));

		as._rep->assign(len2, thirdStr);
#ifndef NDEBUG
		tester(as);
#endif
		assert(as._rep->isShareable());
		assert(as._rep->_refCounter==1);
		assert(as._rep->_len==len2);
		assert(assignmentTest(as));
		assert(accessTest(as));

		cout << "[passed]" << endl << endl << endl;
	}

	template <class T>
	bool isShareable(const AnythingString<T>& as)
	{
		return as._rep->_shareable;
	}

	template <class T>
	void tester(const AnythingString<T>& as)
	{
		cout<<boolalpha;
		cout<<"Shareable: "<<as._rep->_shareable<< endl;
		cout<<"RefCount : "<<as._rep->_refCounter<<endl;
		cout<<"Represent: "<<as._rep<<endl;
	};

	template <class T>
	bool assignmentTest(const AnythingString<T>& as) 
	{
#ifndef NDEBUG
		cout << endl << endl;
		tester(as);
		cout << endl;
		cout << "Tested strings: as(input), as1, as2, as3, as4, as5" << endl;
		cout << "Tests check refCounter and shareable parameters of each string" << endl;
		cout << "a1..5 have differend ways of initialization and assignment" << endl << endl;
#endif
		
		int counter = as._rep->_refCounter;
		AnythingString<T> as1 = as;
		assert(isShareable(as));
		assert(as._rep->_refCounter==++counter);
#ifndef NDEBUG
		cout << "as1 = as [passed]" << endl;
#endif

		AnythingString<T> as2(as);
		assert(isShareable(as)&&isShareable(as2));
		assert(as._rep->_refCounter==++counter);
#ifndef NDEBUG
		cout << "as2(as) [passed]" << endl;
#endif
		
		AnythingString<T> as3 = as1;
		assert(isShareable(as)&&isShareable(as2)&&isShareable(as3));
		assert(as._rep->_refCounter==++counter);
#ifndef NDEBUG
		cout << "as3 = as1 [passed]" << endl;
#endif		

		AnythingString<T> as4 = as2;
		assert(isShareable(as)&&isShareable(as2)&&isShareable(as3)&&isShareable(as4));
		assert(as._rep->_refCounter==++counter);
#ifndef NDEBUG
		cout << "as4 = as2 [passed]" << endl;
#endif		

		AnythingString<T> as5 = as3;
		assert(isShareable(as)&&isShareable(as2)&&isShareable(as3)&&isShareable(as4)&&isShareable(as5));		
		assert(as._rep->_refCounter==++counter);
#ifndef NDEBUG
		cout << "as5 = as3 [passed]" << endl;
#endif		

		as1 = as;
		assert(isShareable(as)&&isShareable(as2)&&isShareable(as3)&&isShareable(as4)&&isShareable(as5));		
		assert(as._rep->_refCounter==counter);
#ifndef NDEBUG
		cout << "as1 = as [passed]" << endl;
#endif		

		as1 = as1;
		assert(isShareable(as)&&isShareable(as2)&&isShareable(as3)&&isShareable(as4)&&isShareable(as5));		
		assert(as._rep->_refCounter==counter);
#ifndef NDEBUG
		cout << "as3 = as1 [passed]" << endl;
#endif		

		as5 = as3;
		assert(isShareable(as)&&isShareable(as2)&&isShareable(as3)&&isShareable(as4)&&isShareable(as5));		
		assert(as._rep->_refCounter==counter);
#ifndef NDEBUG
		cout << "as5 = as3 [passed]" << endl;
		cout << endl;
#endif		
		return true;
	}

	void assignmentTests() 
	{
		cout << "1. Assignment Tests" << endl << endl << endl;

		char* str1 = "Anything string of chars";
		int len1 = strlen(str1);
		AnythingString<char> cas1("Anything string of chars",len1);
		AnythingString<char> cas2("",0);

		int len2 = 10;
		int* str2 = new int[len2];
		for(int i = 0; i < len2; i++)
			str2[i] = i;
		AnythingString<int> ias1(str2,len2);
		AnythingString<int> ias2(new int[0], 0);

		wchar_t* str3 = L"Anything string of wchars";
		int len3 = wcslen(str3);
		AnythingString<wchar_t> was1(str3,len3);
		AnythingString<wchar_t> was2(L"",0);
		
		int len4 = 64;
		ColorChar* str4 = new ColorChar[len4];
		for(int i = 0; i < len4/8; i++)
		{
			for(int j = 0; j < len4/8; j++)
			{
				char c = ((i+j)%2==0) ? '0' : '1';
				Color color = ((i+j)%2==0) ? Yellow : White;
				str4[i*8+j] = ColorChar(c,color);
			}
		}
		AnythingString<ColorChar> ccas1(str4,len4);

		int len5 = 256;
		ColorChar* str5 = new ColorChar[len5];
		for(int i = 0; i < len5; i++)
			str5[i] = ColorChar('c',Color(i));
		AnythingString<ColorChar> ccas2(str5,len5);

		char *a = "ABCDweasd";
		size_t len = length(a);
		size_t * s = new size_t[len];
		for(size_t i=0; i<len; ++i)
			s[i]=i+1;
		FormattedSymbol<size_t> * fs = 0;
		fillFormattedString<size_t>(fs, a, s);
		AnythingString<FormattedSymbol<size_t>> fas(fs, len);

		char* t = " [passed]";
		char* f = " [failed]";
		cout << boolalpha;
		cout << "test for: ";
		cout << cas1;
		cout << (assignmentTest(cas1) ? t : f) << endl;

		cout << "test for: ";
		cout << cas2;
		cout << (assignmentTest(cas2) ? t : f) << endl;
		
		cout << "test for: ";
		cout << ias1;
		cout << (assignmentTest(ias1) ? t : f) << endl;
		
		cout << "test for: ";
		cout << ias2;
		cout << (assignmentTest(ias2) ? t : f) << endl;
		
		cout << "test for: ";
		wcout << was1;
		cout << (assignmentTest(was1) ? t : f) << endl;
		
		cout << "test for: ";
		wcout << was2;
		cout << (assignmentTest(was2) ? t : f) << endl;
		
		cout << "test for: ";
		cout << ccas1;
		cout << (assignmentTest(ccas1) ? t : f) << endl;
		
		cout << "test for: ";
		cout << ccas2;
		cout << (assignmentTest(ccas2) ? t : f) << endl;

		cout << "test for: ";
		cout << fas;
		cout << (assignmentTest(fas) ? t : f) << endl;

		cout << endl << endl;
	}

	template <class T>
	bool accessTest(AnythingString<T>& as) 
	{
#ifndef NDEBUG
		cout << endl << endl;
		cout << "Tests check refCounter and shareable parameters of each string" << endl;
		cout << "Check different ways of string access" << endl << endl;
#endif
		
		int counter = as._rep->_refCounter;
		AnythingString<T> as1 = as;
		++counter;
		assert(isShareable(as)&&isShareable(as1));
		assert(as._rep->_refCounter==counter && as1._rep->_refCounter==counter);

		for(size_t i = 0; i < as.length(); i++)
		{
			cout << as[i] << as1[i];
			assert(isShareable(as)&&isShareable(as1));
			assert(as._rep->_refCounter==counter && as1._rep->_refCounter==counter);
		}
#ifndef NDEBUG
		cout << endl << "as[i] and as1[i] access without unshareabling [passed]" << endl;
#endif
		try {
			as[as.length()+2];
		} catch (BadIndexException e) {
#ifndef NDEBUG
			cout << e.msg() << " at " << e.index() << endl;
#endif
		}

		try {
			as[-1];
		} catch (BadIndexException e) {
#ifndef NDEBUG
			cout << e.msg() << " at " << e.index() << endl;
#endif
		}

		try {
			as1[as1.length()+2];
		} catch (BadIndexException e) {
#ifndef NDEBUG
			cout << e.msg() << " at " << e.index() << endl;
#endif
		}

		try {
			as1[-1];
		} catch (BadIndexException e) {
#ifndef NDEBUG
			cout << e.msg() << " at " << e.index() << endl;
#endif
		}

#ifndef NDEBUG
		cout << "Bad index exception in incorrect ways to access [passed]" << endl;
		cout << endl;
#endif
		return true;
	}

	void accessTests()
	{
		cout << "2. Access Tests" << endl << endl << endl;

		char* str1 = "Anything string of chars";
		int len1 = strlen(str1);
		AnythingString<char> cas1("Anything string of chars",len1);

		int len2 = 10;
		int* str2 = new int[len2];
		for(int i = 0; i < len2; i++)
			str2[i] = i;
		AnythingString<int> ias1(str2,len2);

		wchar_t* str3 = L"Anything string of wchars";
		int len3 = wcslen(str3);
		AnythingString<wchar_t> was1(str3,len3);
		
		int len4 = 64;
		ColorChar* str4 = new ColorChar[len4];
		for(int i = 0; i < len4/8; i++)
		{
			for(int j = 0; j < len4/8; j++)
			{
				char c = ((i+j)%2==0) ? '0' : '1';
				Color color = ((i+j)%2==0) ? Yellow : White;
				str4[i*8+j] = ColorChar(c,color);
			}
		}
		AnythingString<ColorChar> ccas1(str4,len4);

		char *a = "ABCD";
		size_t len = length(a);
		size_t * s = new size_t[len];
		for(size_t i=0; i<len; ++i)
			s[i]=i+1;
		FormattedSymbol<size_t> * fs = 0;
		fillFormattedString<size_t>(fs, a, s);
		AnythingString<FormattedSymbol<size_t>> fas(fs, len);

		char* t = " [passed]";
		char* f = " [failed]";

		cout << boolalpha;
		cout << "test for: ";
		cout << cas1;
		cout << (accessTest(cas1) ? t : f) << endl;

		cout << "test for: ";
		cout << ias1;
		cout << (accessTest(ias1) ? t : f) << endl;
		
		cout << "test for: ";
		wcout << was1;
		cout << (accessTest(was1) ? t : f) << endl;
		
		cout << "test for: ";
		cout << ccas1;
		cout << (accessTest(ccas1) ? t : f) << endl;
		
		cout << "test for: ";
		cout << fas;
		cout << (accessTest(fas) ? t : f) << endl;

		cout << endl << endl;
	}

	void undefinedPointerTest()
	{
		cout << "3. Undefined pointer test" << endl << endl << endl;

		try {
		AnythingString<char> as1(0,0);
		} catch (UndefinedPointerException upe) {
			cout << upe << " [passed]" << endl;
		}
		try {
		AnythingString<int> as2(0,0);
		} catch (UndefinedPointerException upe) {
			cout << upe << " [passed]" << endl;
		}
		try {
		AnythingString<wchar_t> was2(0,0);
		} catch (UndefinedPointerException upe) {
			cout << upe << " [passed]" << endl;
		}
		try {
		AnythingString<ColorChar> ccas1(0,0);
		} catch (UndefinedPointerException upe) {
			cout << upe << " [passed]" << endl;
		}

		cout << endl << endl;
	}

	void ñonstCornerCaseTest()
	{
		cout << "4. Const Corner Case Test" << endl << endl << endl;

		char* str1 = "Anything string of chars";
		int len1 = strlen(str1);
		const AnythingString<char> cas1("Anything string of chars",len1);
		const AnythingString<char> cas11(cas1);
		assert(cas11._rep->_refCounter == 2);
		char c1 = cas1[0];
		assert(cas11._rep->_refCounter == 2);
		assert(isShareable(cas1));
		cout << "char c1 = cas1[0]; [passed]" << endl;
		cout << "ref counter not decreased [passed]" << endl;

		const AnythingString<char> cas2("Anything string of chars",len1);
		const char& c2 = cas2[0];
		assert(isShareable(cas2));
		cout << "const char& c2 = cas2[0]; [passed]" << endl;

		const AnythingString<char> cas3("Anything string of chars",len1);
		//char& c3 = cas3[0]; impossible
		assert(isShareable(cas3));
		cout << "char& c3 = cas3[0]; [passed]" << endl;
		
		const AnythingString<char> cas4("Anything string of chars",len1);
		const char* c4 = &cas4[0];
		assert(isShareable(cas4));
		cout << "const char* c4 = &cas4[0]; [passed]" << endl;

		const AnythingString<char> cas5("Anything string of chars",len1);
		//char* c5 = &cas5[0]; impossible
		assert(isShareable(cas5));
		cout << "char* c5 = &cas5[0]; [passed]" << endl;

		assert(isShareable(cas1)&&isShareable(cas2)&&isShareable(cas3)&&isShareable(cas4)&&isShareable(cas5));
		cout << "cas1..5 sharable checking [passed]" << endl;

		cout << endl << endl;
	}
	
	/*
	We have several problems because acces in C++ assignment for non-const objects
	will in first priority use non-static functions (if he has) even if we have const functions.
	So, in our case it means, that proxy will always return non-const T& value
	for non-const objects and it will break object sharability.
	We have only one solution in this case - remove non-const functions, but we can't, because we use them.
	*/
	void nonConstCornerCaseTest()
	{
		cout << "5. Non-Const Corner Case Test" << endl << endl << endl;

		char* str1 = "Anything string of chars";
		int len1 = strlen(str1);
		AnythingString<char> cas1("Anything string of chars",len1);
		AnythingString<char> cas11(cas1);
		assert(cas11._rep->_refCounter == 2);
		char c1 = cas1[0];
		assert(cas11._rep->_refCounter == 1);
		assert(!isShareable(cas1));
		cout << "char c1 = cas1[0]; [failed]" << endl;
		cout << "ref counter decrease [passed]" << endl;

		AnythingString<char> cas2("Anything string of chars",len1);
		const char& c2 = cas2[0];
		assert(!isShareable(cas2));
		cout << "const char& c2 = cas2[0]; [failed]" << endl;

		AnythingString<char> cas3("Anything string of chars",len1);
		char& c3 = cas3[0];
		assert(!isShareable(cas3));
		cout << "char& c3 = cas3[0]; [failed]" << endl;
		
		AnythingString<char> cas4("Anything string of chars",len1);
		const char* c4 = &cas4[0];
		assert(!isShareable(cas4));
		cout << "const char* c4 = &cas4[0]; [failed]" << endl;

		AnythingString<char> cas5("Anything string of chars",len1);
		char* c5 = &cas5[0];
		assert(!isShareable(cas5));
		cout << "char* c5 = &cas5[0]; [failed]" << endl;

		assert(!isShareable(cas1)&&!isShareable(cas2)&&!isShareable(cas3)&&!isShareable(cas4)&&!isShareable(cas5));
		cout << "cas1..5 sharable checking [failed]" << endl;

		cout << endl << endl;
	}

};

#endif