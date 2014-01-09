// compare boost rand functions for speed (not distribution test)

// random_cmp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <conio.h>

#include <Windows.h>
#include <vector>
#include <boost/random.hpp>
#include <numeric>

using namespace std;
using namespace boost;

template <typename TResult, int _Count__ = 10>
class rresult
{
public:
	std::vector<TResult> res;
	std::vector<uint64_t> t1;
	std::vector<uint64_t> t2;

	rresult() {
		res.reserve(_Count__);
		t1.reserve(_Count__);
		t2.reserve(_Count__);
	}
};

class hdtimer
{
	LARGE_INTEGER x1, x2, x3;

public:
	void start()
	{
		QueryPerformanceCounter(&x1);
	}

	void lap() // :)
	{
		QueryPerformanceCounter(&x3);
	}

	void stop()
	{
		QueryPerformanceCounter(&x2);
	}

	unsigned __int64 get() const
	{
		return x2.QuadPart - x1.QuadPart;
	}

	unsigned __int64 get_lap() const
	{
		return x2.QuadPart - x3.QuadPart;
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	hdtimer x;

	const int trou(100000);

	rresult<int32_t, trou> v_minstd_rand;
	rresult<int32_t, trou> v_minstd_rand0;
	rresult<int32_t, trou> v_rand48;
	rresult<int32_t, trou> v_ecuyer1988;
	rresult<int32_t, trou> v_hellekalek1995;

	rresult<uint32_t, trou> v_kreutzer1986;
	rresult<uint32_t, trou> v_taus88;
	rresult<uint32_t, trou> v_mt11213b;
	rresult<uint32_t, trou> v_mt19937;

	rresult<double, trou> v_lagged_fibonacci607;
	rresult<double, trou> v_lagged_fibonacci44497;

	rresult<int, trou> v_ranlux4;
	rresult<int, trou> v_ranlux3;

	for(int rl = trou; rl != 0; rl--)
	{
		{
			x.start();
			minstd_rand _minstd_rand;
			_minstd_rand.seed(rl);
			x.lap();
			v_minstd_rand.res.push_back( _minstd_rand() );
			x.stop();
			v_minstd_rand.t1.push_back( x.get_lap() );
			v_minstd_rand.t2.push_back( x.get() );
		}

		{
			x.start();
			minstd_rand0 _minstd_rand0;
			_minstd_rand0.seed(rl);
			x.lap();
			v_minstd_rand0.res.push_back( _minstd_rand0() );
			x.stop();
			v_minstd_rand0.t1.push_back( x.get_lap() );
			v_minstd_rand0.t2.push_back( x.get() );
		}

		{
			x.start();
			rand48 _rand48;
			_rand48.seed((int32_t)rl);
			x.lap();
			v_rand48.res.push_back( _rand48() );
			x.stop();
			v_rand48.t1.push_back( x.get_lap() );
			v_rand48.t2.push_back( x.get() );
		}

		{
			x.start();
			ecuyer1988 _ecuyer1988;
			_ecuyer1988.seed();
			x.lap();
			v_ecuyer1988.res.push_back( _ecuyer1988() );
			x.stop();
			v_ecuyer1988.t1.push_back( x.get_lap() );
			v_ecuyer1988.t2.push_back( x.get() );
		}

		{
			x.start();
			kreutzer1986 _kreutzer1986;
			_kreutzer1986.seed(rl);
			x.lap();
			v_kreutzer1986.res.push_back( _kreutzer1986() );
			x.stop();
			v_kreutzer1986.t1.push_back( x.get_lap() );
			v_kreutzer1986.t2.push_back( x.get() );
		}

		{
			x.start();
			taus88 _taus88;
			_taus88.seed();
			x.lap();
			v_taus88.res.push_back( _taus88() );
			x.stop();
			v_taus88.t1.push_back( x.get_lap() );
			v_taus88.t2.push_back( x.get() );
		}

		{
			x.start();
			hellekalek1995 _hellekalek1995;
			_hellekalek1995.seed(rl);
			x.lap();
			v_hellekalek1995.res.push_back( _hellekalek1995() );
			x.stop();
			v_hellekalek1995.t1.push_back( x.get_lap() );
			v_hellekalek1995.t2.push_back( x.get() );
		}

		{
			x.start();
			mt11213b _mt11213b;
			_mt11213b.seed();
			x.lap();
			v_mt11213b.res.push_back( _mt11213b() );
			x.stop();
			v_mt11213b.t1.push_back( x.get_lap() );
			v_mt11213b.t2.push_back( x.get() );
		}

		{
			x.start();
			mt19937 _mt19937;
			_mt19937.seed();
			x.lap();
			v_mt19937.res.push_back( _mt19937() );
			x.stop();
			v_mt19937.t1.push_back( x.get_lap() );
			v_mt19937.t2.push_back( x.get() );
		}

		{
			x.start();
			lagged_fibonacci607 _lagged_fibonacci607;
			_lagged_fibonacci607.seed();
			x.lap();
			v_lagged_fibonacci607.res.push_back( _lagged_fibonacci607() );
			x.stop();
			v_lagged_fibonacci607.t1.push_back( x.get_lap() );
			v_lagged_fibonacci607.t2.push_back( x.get() );
		}

		{
			x.start();
			lagged_fibonacci44497 _lagged_fibonacci44497;
			_lagged_fibonacci44497.seed();
			x.lap();
			v_lagged_fibonacci44497.res.push_back( _lagged_fibonacci44497() );
			x.stop();
			v_lagged_fibonacci44497.t1.push_back( x.get_lap() );
			v_lagged_fibonacci44497.t2.push_back( x.get() );
		}

		{
			x.start();
			ranlux3 _ranlux3;
			_ranlux3.seed();
			x.lap();
			v_ranlux3.res.push_back( _ranlux3() );
			x.stop();
			v_ranlux3.t1.push_back( x.get_lap() );
			v_ranlux3.t2.push_back( x.get() );
		}

		{
			x.start();
			ranlux4 _ranlux4;
			_ranlux4.seed();
			x.lap();
			v_ranlux4.res.push_back( _ranlux4() );
			x.stop();
			v_ranlux4.t1.push_back( x.get_lap() );
			v_ranlux4.t2.push_back( x.get() );
		}
	}

	printf("==== minstd_rand ====\naverage generation %.5f\nmax time %I64d\nmin time %I64d\ninitialization %.5f\n\n"
		, std::accumulate(v_minstd_rand.t1.begin(), v_minstd_rand.t1.end(), 0.0f) / trou
		, *std::max_element(v_minstd_rand.t1.begin(), v_minstd_rand.t1.end())
		, *std::min_element(v_minstd_rand.t1.begin(), v_minstd_rand.t1.end())
		, std::accumulate(v_minstd_rand.t2.begin(), v_minstd_rand.t2.end(), 0.0f) / trou
		);

	printf("==== minstd_rand0 ====\naverage generation %.5f\nmax time %I64d\nmin time %I64d\ninitialization %.5f\n\n"
		, std::accumulate(v_minstd_rand0.t1.begin(), v_minstd_rand0.t1.end(), 0.0f) / trou
		, *std::max_element(v_minstd_rand0.t1.begin(), v_minstd_rand0.t1.end())
		, *std::min_element(v_minstd_rand0.t1.begin(), v_minstd_rand0.t1.end())
		, std::accumulate(v_minstd_rand0.t2.begin(), v_minstd_rand0.t2.end(), 0.0f) / trou
		);

	printf("==== rand48 ====\naverage generation %.5f\nmax time %I64d\nmin time %I64d\ninitialization %.5f\n\n"
		, std::accumulate(v_rand48.t1.begin(), v_rand48.t1.end(), 0.0f) / trou
		, *std::max_element(v_rand48.t1.begin(), v_rand48.t1.end())
		, *std::min_element(v_rand48.t1.begin(), v_rand48.t1.end())
		, std::accumulate(v_rand48.t2.begin(), v_rand48.t2.end(), 0.0f) / trou
		);

	printf("==== ecuyer1988 ====\naverage generation %.5f\nmax time %I64d\nmin time %I64d\ninitialization %.5f\n\n"
		, std::accumulate(v_ecuyer1988.t1.begin(), v_ecuyer1988.t1.end(), 0.0f) / trou
		, *std::max_element(v_ecuyer1988.t1.begin(), v_ecuyer1988.t1.end())
		, *std::min_element(v_ecuyer1988.t1.begin(), v_ecuyer1988.t1.end())
		, std::accumulate(v_ecuyer1988.t2.begin(), v_ecuyer1988.t2.end(), 0.0f) / trou
		);

	printf("==== kreutzer1986 ====\naverage generation %.5f\nmax time %I64d\nmin time %I64d\ninitialization %.5f\n\n"
		, std::accumulate(v_kreutzer1986.t1.begin(), v_kreutzer1986.t1.end(), 0.0f) / trou
		, *std::max_element(v_kreutzer1986.t1.begin(), v_kreutzer1986.t1.end())
		, *std::min_element(v_kreutzer1986.t1.begin(), v_kreutzer1986.t1.end())
		, std::accumulate(v_kreutzer1986.t2.begin(), v_kreutzer1986.t2.end(), 0.0f) / trou
		);

	printf("==== taus88 ====\naverage generation %.5f\nmax time %I64d\nmin time %I64d\ninitialization %.5f\n\n"
		, std::accumulate(v_taus88.t1.begin(), v_taus88.t1.end(), 0.0f) / trou
		, *std::max_element(v_taus88.t1.begin(), v_taus88.t1.end())
		, *std::min_element(v_taus88.t1.begin(), v_taus88.t1.end())
		, std::accumulate(v_taus88.t2.begin(), v_taus88.t2.end(), 0.0f) / trou
		);

	printf("==== hellekalek1995 ====\naverage generation %.5f\nmax time %I64d\nmin time %I64d\ninitialization %.5f\n\n"
		, std::accumulate(v_hellekalek1995.t1.begin(), v_hellekalek1995.t1.end(), 0.0f) / trou
		, *std::max_element(v_hellekalek1995.t1.begin(), v_hellekalek1995.t1.end())
		, *std::min_element(v_hellekalek1995.t1.begin(), v_hellekalek1995.t1.end())
		, std::accumulate(v_hellekalek1995.t2.begin(), v_hellekalek1995.t2.end(), 0.0f) / trou
		);

	printf("==== mt11213b ====\naverage generation %.5f\nmax time %I64d\nmin time %I64d\ninitialization %.5f\n\n"
		, std::accumulate(v_mt11213b.t1.begin(), v_mt11213b.t1.end(), 0.0f) / trou
		, *std::max_element(v_mt11213b.t1.begin(), v_mt11213b.t1.end())
		, *std::min_element(v_mt11213b.t1.begin(), v_mt11213b.t1.end())
		, std::accumulate(v_mt11213b.t2.begin(), v_mt11213b.t2.end(), 0.0f) / trou
		);

	printf("==== mt19937 ====\naverage generation %.5f\nmax time %I64d\nmin time %I64d\ninitialization %.5f\n\n"
		, std::accumulate(v_mt19937.t1.begin(), v_mt19937.t1.end(), 0.0f) / trou
		, *std::max_element(v_mt19937.t1.begin(), v_mt19937.t1.end())
		, *std::min_element(v_mt19937.t1.begin(), v_mt19937.t1.end())
		, std::accumulate(v_mt19937.t2.begin(), v_mt19937.t2.end(), 0.0f) / trou
		);

	printf("==== lagged_fibonacci607 ====\naverage generation %.5f\nmax time %I64d\nmin time %I64d\ninitialization %.5f\n\n"
		, std::accumulate(v_lagged_fibonacci607.t1.begin(), v_lagged_fibonacci607.t1.end(), 0.0f) / trou
		, *std::max_element(v_lagged_fibonacci607.t1.begin(), v_lagged_fibonacci607.t1.end())
		, *std::min_element(v_lagged_fibonacci607.t1.begin(), v_lagged_fibonacci607.t1.end())
		, std::accumulate(v_lagged_fibonacci607.t2.begin(), v_lagged_fibonacci607.t2.end(), 0.0f) / trou
		);

	printf("==== lagged_fibonacci44497 ====\naverage generation %.5f\nmax time %I64d\nmin time %I64d\ninitialization %.5f\n\n"
		, std::accumulate(v_lagged_fibonacci44497.t1.begin(), v_lagged_fibonacci44497.t1.end(), 0.0f) / trou
		, *std::max_element(v_lagged_fibonacci44497.t1.begin(), v_lagged_fibonacci44497.t1.end())
		, *std::min_element(v_lagged_fibonacci44497.t1.begin(), v_lagged_fibonacci44497.t1.end())
		, std::accumulate(v_lagged_fibonacci44497.t2.begin(), v_lagged_fibonacci44497.t2.end(), 0.0f) / trou
		);

	printf("==== ranlux3 ====\naverage generation %.5f\nmax time %I64d\nmin time %I64d\ninitialization %.5f\n\n"
		, std::accumulate(v_ranlux3.t1.begin(), v_ranlux3.t1.end(), 0.0f) / trou
		, *std::max_element(v_ranlux3.t1.begin(), v_ranlux3.t1.end())
		, *std::min_element(v_ranlux3.t1.begin(), v_ranlux3.t1.end())
		, std::accumulate(v_ranlux3.t2.begin(), v_ranlux3.t2.end(), 0.0f) / trou
		);

	printf("==== ranlux4 ====\naverage generation %.5f\nmax time %I64d\nmin time %I64d\ninitialization %.5f\n\n"
		, std::accumulate(v_ranlux4.t1.begin(), v_ranlux4.t1.end(), 0.0f) / trou
		, *std::max_element(v_ranlux4.t1.begin(), v_ranlux4.t1.end())
		, *std::min_element(v_ranlux4.t1.begin(), v_ranlux4.t1.end())
		, std::accumulate(v_ranlux4.t2.begin(), v_ranlux4.t2.end(), 0.0f) / trou
		);

//	getch();

	return 0;
}
