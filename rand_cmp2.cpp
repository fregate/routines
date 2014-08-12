// test default and new (std::tr1) random engines
// if remove wrappers (virtual functions exactly) in will be faster at all

#include "stdafx.h"

#include <memory>
#include <sstream>
#include <iostream>

#include <random>
#include <boost/random.hpp>

using namespace std;

volatile bool done = false;

template <unsigned int D>
int d_re(const unsigned __int64& rn) // roll dice with D edges (cut input in range, in fact)
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

	unsigned __int64 sum2_re;
	unsigned __int64 sum3_re;

	re_i()
		: sum_re(0)
		, max_re(0)
		, min_re(-1)
		, sum2_re(0)
		, sum3_re(0)
	{}

	virtual int get_boost_distr()  = 0;
	virtual int get_std_distr()  = 0;
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

template <class RE, unsigned int D = 17>
struct re : public re_i
{
	RE x;
	boost::random::uniform_int_distribution<> _boost_distr;
	uniform_int_distribution<> _std_distr;

	re()
		: _boost_distr(1, D)
		, _std_distr(1, D)
	{
		x.seed(::GetTickCount());
	}

	virtual unsigned __int64 get()
	{
		return x();
	}

	virtual int get_boost_distr()
	{
		return _boost_distr(x);
	}

	virtual int get_std_distr()
	{
		return _std_distr(x);
	}
};

struct DefRE
{ // using default rand() function

};

template<int D>
struct re<DefRE, D> : public re_i
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

	virtual int get_boost_distr()
	{
		return d_re<D>(rand());
	}

	virtual int get_std_distr()
	{
		return d_re<D>(rand());
	}
};

template<int D> // fibbonachi random engine return double[0...1] - conversion for plain function
struct re<boost::random::lagged_fibonacci607, D> : public re_i
{
	boost::random::lagged_fibonacci607 x;
	boost::random::uniform_int_distribution<> _boost_distr;
	uniform_int_distribution<> _std_distr;

	re()
		: _boost_distr(1, D)
		, _std_distr(1, D)
	{
		x.seed(::GetTickCount());
	}

	unsigned __int64 sum_re;

	virtual unsigned __int64 get()
	{
		return (unsigned __int64) (x() * UINT64_MAX);
	}

	virtual int get_boost_distr()
	{
		return _boost_distr(x);
	}

	virtual int get_std_distr()
	{
		return _std_distr(x);
	}
};

template<int D>
struct re<boost::random::lagged_fibonacci44497, D> : public re_i
{
	boost::random::lagged_fibonacci607 x;
	boost::random::uniform_int_distribution<> _boost_distr;
	uniform_int_distribution<> _std_distr;

	re()
		: _boost_distr(1, D)
		, _std_distr(1, D)
	{
		x.seed(::GetTickCount());
	}

	virtual unsigned __int64 get()
	{
		return (unsigned __int64) (x() * UINT64_MAX);
	}

	virtual int get_boost_distr()
	{
		return _boost_distr(x);
	}

	virtual int get_std_distr()
	{
		return _std_distr(x);
	}
};

template<int D>
struct re<knuth_b, D> : public re_i // didn't work with uniform_distr
{
	knuth_b x;

	re()
	{
		x.seed(::GetTickCount());
	}

	virtual unsigned __int64 get()
	{
		return x();
	}

	virtual int get_boost_distr()
	{
		return d_re<D>(x()); 
	}

	virtual int get_std_distr()
	{
		return d_re<D>(x());
	}
};

void re_test(const unsigned __int64& i, re_i& z, const std::string& comm)
{
	LARGE_INTEGER previous;
	LARGE_INTEGER current;

	{
		QueryPerformanceCounter(&previous);
		unsigned __int64 x(d_re<17>(z.get()));
		QueryPerformanceCounter(&current);
		z.sum(current.QuadPart - previous.QuadPart);

		z.max_re = z.max_re > x ? z.max_re : x;
		z.min_re = z.min_re < x ? z.min_re : x; // check for boundary conditions

		wostringstream oss;
		oss << "d_re<" << comm.c_str() << "> info [current: " << current.QuadPart - previous.QuadPart
			<< "][avg:" << (double) z.show() / i << "]" << endl;
		OutputDebugString(oss.str().c_str());
	}

	{
		QueryPerformanceCounter(&previous);
		z.get_boost_distr();
		QueryPerformanceCounter(&current);
		z.sum2_re += current.QuadPart - previous.QuadPart;

		wostringstream oss;
		oss << "d_re<" << comm.c_str() << "> boost_distr info [current: " << current.QuadPart - previous.QuadPart
			<< "][avg:" << (double) z.sum2_re / i << "]" << endl;
		OutputDebugString(oss.str().c_str());
	}

	{
		QueryPerformanceCounter(&previous);
		z.get_std_distr();
		QueryPerformanceCounter(&current);
		z.sum3_re += current.QuadPart - previous.QuadPart;

		wostringstream oss;
		oss << "d_re<" << comm.c_str() << "> std_distr info [current: " << current.QuadPart - previous.QuadPart
			<< "][avg:" << (double) z.sum3_re / i << "]" << endl;
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
	
	re<boost::random::minstd_rand> _boost_minstd_rand;
	re<boost::random::minstd_rand0> _boost_minstd_rand0;
	re<boost::random::rand48> _boost_rand48;
	re<boost::random::ecuyer1988> _boost_ecuyer1988;
	re<boost::random::hellekalek1995> _boost_hellekalek1995;
	re<boost::random::kreutzer1986> _boost_kreutzer1986;
	re<boost::random::taus88> _boost_taus88;
	re<boost::random::mt11213b> _boost_mt11213b;
	re<boost::random::mt19937> _boost_mt19937;
	re<boost::random::lagged_fibonacci607> _boost_lagged_fibonacci607;
	re<boost::random::lagged_fibonacci44497> _boost_lagged_fibonacci44497;
	re<boost::random::ranlux4> _boost_ranlux4;
	re<boost::random::ranlux3> _boost_ranlux3;

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
		
		RE_TEST(boost_minstd_rand);
		RE_TEST(boost_minstd_rand0);
		RE_TEST(boost_rand48);
		RE_TEST(boost_ecuyer1988);
		RE_TEST(boost_hellekalek1995);
		RE_TEST(boost_kreutzer1986);
		RE_TEST(boost_taus88);
		RE_TEST(boost_mt11213b);
		RE_TEST(boost_mt19937);
		RE_TEST(boost_lagged_fibonacci607);
		RE_TEST(boost_lagged_fibonacci44497);
		RE_TEST(boost_ranlux4);
		RE_TEST(boost_ranlux3);
	}

#define OUTPUT_RE_STAT(c) \
	cout << "d<" << #c << "> avg [" << (double) _##c.show() / lag << "][min: " << _##c.min_re << "][max: " << _##c.max_re << "] "; \
	cout << "boost_distr avg [" << (double) _##c.sum2_re / lag << "] "; \
	cout << "std_distr avg [" << (double) _##c.sum3_re / lag << "]" << endl; 

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
	
	OUTPUT_RE_STAT(boost_minstd_rand);
	OUTPUT_RE_STAT(boost_minstd_rand0);
	OUTPUT_RE_STAT(boost_rand48);
	OUTPUT_RE_STAT(boost_ecuyer1988);
	OUTPUT_RE_STAT(boost_hellekalek1995);
	OUTPUT_RE_STAT(boost_kreutzer1986);
	OUTPUT_RE_STAT(boost_taus88);
	OUTPUT_RE_STAT(boost_mt11213b);
	OUTPUT_RE_STAT(boost_mt19937);
	OUTPUT_RE_STAT(boost_lagged_fibonacci607);
	OUTPUT_RE_STAT(boost_lagged_fibonacci44497);
	OUTPUT_RE_STAT(boost_ranlux4);
	OUTPUT_RE_STAT(boost_ranlux3);

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

d<def> avg                              [0.433816][min: 1][max: 17]
d<minstd_rand0> avg                     [0.177613][min: 1][max: 17]
d<minstd_rand> avg                      [0.169577][min: 1][max: 17]
d<mt19937> avg                          [0.151529][min: 1][max: 17] **
d<mt19937_64> avg                       [0.253063][min: 1][max: 17]
d<ranlux24_base> avg                    [0.200032][min: 1][max: 17]
d<ranlux48_base> avg                    [0.234577][min: 1][max: 17]
d<ranlux24> avg                         [0.472101][min: 1][max: 17]
d<ranlux48> avg                         [1.25102][min: 1][max: 17]
d<knuth_b> avg                          [0.247116][min: 1][max: 17]
d<default_random_engine> avg            [0.162573][min: 1][max: 17]
d<boost_minstd_rand> avg                [0.17204][min: 1][max: 17]
d<boost_minstd_rand0> avg               [0.186877][min: 1][max: 17]
d<boost_rand48> avg                     [0.16532][min: 1][max: 17]
d<boost_ecuyer1988> avg                 [0.212816][min: 1][max: 17]
d<boost_hellekalek1995> avg             [0.624603][min: 1][max: 17]
d<boost_kreutzer1986> avg               [0.144129][min: 1][max: 17] * - fastest
d<boost_taus88> avg                     [0.161525][min: 1][max: 17]
d<boost_mt11213b> avg                   [0.170108][min: 1][max: 17]
d<boost_mt19937> avg                    [0.155041][min: 1][max: 17] ***
d<boost_lagged_fibonacci607> avg        [0.319005][min: 1][max: 17]
d<boost_lagged_fibonacci44497> avg      [0.31203][min: 1][max: 17]
d<boost_ranlux4> avg                    [0.559111][min: 1][max: 17]
d<boost_ranlux3> avg                    [0.413897][min: 1][max: 17]
iterations [6`145`290]
*/
