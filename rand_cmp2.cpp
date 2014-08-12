// ttt.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <memory>
#include <sstream>
#include <iostream>

#include <random>

using namespace std;

volatile bool done = false;

template <unsigned int D>
int d_re(const unsigned __int64& rn)
{
	if(D)
	{
		return (rn % D) + 1;
	}

	return 0;
}

void processInput()
{
	done = (GetAsyncKeyState(0x58) & 0x8000) != 0;
}

struct re_i
{
	unsigned __int64 sum_re;
	unsigned __int64 max_re;
	unsigned __int64 min_re;

	re_i()
		: sum_re(0)
		, max_re(0)
		, min_re(-1)
	{}

	virtual unsigned __int64 get()  = 0;
	virtual void sum(const unsigned __int64& s)
	{
		sum_re += s;
	};

	virtual unsigned __int64 show()
	{
		return sum_re;
	};
};

template <class RE>
struct re : public re_i
{
	RE x;

	re()
	{
		x.seed(::GetTickCount());
	}

	virtual unsigned __int64 get()
	{
		return x();
	}
};

struct DefRE
{ // using default rand() function

};

template<>
struct re<DefRE> : public re_i
{
	unsigned __int64 sum_re;

	re() : sum_re(0)
	{
		srand(::GetTickCount());
	}

	virtual unsigned __int64 get()
	{
		return rand();
	}
};

void re_test(const unsigned __int64& i, re_i& z, const std::string& comm)
{
	LARGE_INTEGER previous;
	LARGE_INTEGER current;

	QueryPerformanceCounter(&previous);
	unsigned __int64 x(d_re<17>(z.get()));
	QueryPerformanceCounter(&current);
	z.sum(current.QuadPart - previous.QuadPart);

	z.max_re = z.max_re > x ? z.max_re : x;
	z.min_re = z.min_re < x ? z.min_re : x; // check for boundary conditions

	{
		wostringstream oss;
		oss << "d_re<" << comm.c_str() << "> info [current: " << current.QuadPart - previous.QuadPart
			<< "][avg:" << (double) z.show() / i << "]" << endl;
		OutputDebugString(oss.str().c_str());
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	re<DefRE> _def;
	re<minstd_rand0> _minstd_rand0;
	re<minstd_rand> _minstd_rand;
	re<mt19937> _mt19937;
	re<mt19937_64> _mt19937_64;
	re<ranlux24_base> _ranlux24_base;
	re<ranlux48_base> _ranlux48_base;
	re<ranlux24> _ranlux24;
	re<ranlux48> _ranlux48;
	re<knuth_b> _knuth_b;
	re<default_random_engine> _default_random_engine;

	__int64 lag(0);

	while (!done)
	{
		processInput();
		lag++;

#define RE_TEST(c) \
	re_test(lag, _##c, #c);

		RE_TEST(def);
		RE_TEST(minstd_rand0);
		RE_TEST(minstd_rand);
		RE_TEST(mt19937);
		RE_TEST(mt19937_64);
		RE_TEST(ranlux24_base);
		RE_TEST(ranlux48_base);
		RE_TEST(ranlux24);
		RE_TEST(ranlux48);
		RE_TEST(knuth_b);
		RE_TEST(default_random_engine);
	}

#define OUTPUT_RE_STAT(c) \
	cout << "d<" << #c << "> avg [" << (double) _##c.show() / lag << "][min: " << _##c.min_re << "][max: " << _##c.max_re << "]" << endl; 

	OUTPUT_RE_STAT(def);
	OUTPUT_RE_STAT(minstd_rand0);
	OUTPUT_RE_STAT(minstd_rand);
	OUTPUT_RE_STAT(mt19937);
	OUTPUT_RE_STAT(mt19937_64);
	OUTPUT_RE_STAT(ranlux24_base);
	OUTPUT_RE_STAT(ranlux48_base);
	OUTPUT_RE_STAT(ranlux24);
	OUTPUT_RE_STAT(ranlux48);
	OUTPUT_RE_STAT(knuth_b);
	OUTPUT_RE_STAT(default_random_engine);
	cout << "iterations [" << lag << "]" << endl;

	return 0;
}

/*
d<def> avg                    [0.349123]
d<minstd_rand0> avg           [0.152279]
d<minstd_rand> avg            [0.114103] * - fastest
d<mt19937> avg                [0.129373] **
d<mt19937_64> avg             [0.152789]
d<ranlux24_base> avg          [0.153768]
d<ranlux48_base> avg          [0.19417]
d<ranlux24> avg               [0.383073]
d<ranlux48> avg               [1.15691]
d<knuth_b> avg                [0.17827]
d<default_random_engine> avg  [0.138237] ***
iterations [395`853`725]
*/
