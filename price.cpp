
#include "price.h"


int64_t managed::checked_addition(const int64_t& lhs, const int64_t& rhs)
{
	if (std::numeric_limits<int64_t>::max() - lhs < rhs)
		throw managed::price_overflow_exc("addition overflow");

	return lhs + rhs;
}

int64_t managed::checked_subtraction(const int64_t& lhs, const int64_t& rhs)
{
	if (lhs < (std::numeric_limits<int64_t>::min() + rhs))
		throw managed::price_overflow_exc("subtraction overflow");

	return lhs - rhs;
}

int64_t managed::checked_mult(const int64_t& lhs, const long double& rhs)
{
	if ((std::numeric_limits<int64_t>::max() / rhs) < lhs)
		throw managed::price_overflow_exc("multiplication overflow");

	return static_cast<int64_t>(rhs * lhs);
}

int64_t managed::checked_assign(const long double& rhs)
{
	if (std::numeric_limits<int64_t>::max() < rhs)
		throw managed::price_overflow_exc("assign from double overflow");

	return static_cast<int64_t>(rhs);
}
