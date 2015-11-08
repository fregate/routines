
#pragma once

#include <stdint.h>
#include <math.h>
#include <limits>
#include <exception>

struct managed
{
	typedef std::exception price_overflow_exc;

	static int64_t checked_addition(const int64_t& lhs, const int64_t& rhs);
	static int64_t checked_subtraction(const int64_t& lhs, const int64_t& rhs);
	static int64_t checked_mult(const int64_t& lhs, const long double& rhs);
	static int64_t checked_assign(const long double& rhs);
};

struct unmanaged {};

template<typename M>
struct price_t
{
	int64_t m_price;
	unsigned short m_scale;

	price_t()
		: m_price(0)
		, m_scale(0)
	{}

	price_t(int64_t p, unsigned short s)
		: m_price(p)
		, m_scale(s)
	{}

	explicit price_t(double d)
	{
		assign(d);
	}

	explicit price_t(int64_t i64)
		: m_price(i64)
		, m_scale(0)
	{}

	price_t(const price_t& p)
	{
		m_price = p.m_price;
		m_scale = p.m_scale;
	}

	explicit price_t(unsigned int value)
		: m_price(static_cast<int64_t>(value))
		, m_scale(0)
	{}
	explicit price_t(int value)
		: m_price(static_cast<int64_t>(value))
		, m_scale(0)
	{}

	explicit price_t(float value)
	{
		assign(value);
	}

	price_t & operator=(const price_t &rhs)
	{
		if (&rhs != this)
		{
			m_price = rhs.m_price;
			m_scale = rhs.m_scale;
		}
		return *this;
	}

	price_t & operator=(int64_t rhs)
	{
		m_price = static_cast<int64_t>(rhs * pow(10, m_scale));
		return *this;
	}

	price_t & operator=(int rhs)
	{
		m_price = static_cast<int64_t>(rhs * pow(10, m_scale));
		return *this;
	}

	price_t & operator=(double rhs)
	{
		unsigned short s(m_scale);
		assign(rhs);
		change_scale(s);
		return *this;
	}

	// math operators
	const price_t operator + (const price_t& p) const
	{
		price_t<M> res(*this);
		res.m_price = static_cast<int64_t>((res.get_integral() + p.get_integral()) * pow(10, res.m_scale));
		res.m_price += res.m_scale < p.m_scale
			? get_fractional(p.m_scale - res.m_scale) + p.get_fractional()
			: get_fractional() + p.get_fractional(res.m_scale - p.m_scale);

		return res;
	}

	price_t& operator += (const price_t& p)
	{
		*this = this->operator+(p);
		return *this;
	}

	const price_t operator+() const
	{
		return *this;
	}

	const price_t operator-() const
	{
		price_t res = *this;
		res.m_price = -res.m_price;
		return res;
	}

	const price_t operator - (const price_t& p) const
	{
		price_t res(*this);
		res.m_price = static_cast<int64_t>((res.get_integral() - p.get_integral()) * pow(10, res.m_scale));
		res.m_price += res.m_scale < p.m_scale
			? get_fractional(p.m_scale - res.m_scale) - p.get_fractional()
			: get_fractional() - p.get_fractional(res.m_scale - p.m_scale);

		return res;
	}

	price_t& operator -= (const price_t& p)
	{
		*this = this->operator-(p);
		return *this;
	}

	const price_t operator * (const double& p) const // in-scale multiplication
	{
		price_t res(*this);
		unsigned short s(res.m_scale);
		res.assign(res.get_double() * p);
		res.change_scale(s);

		return res;
	}

	price_t& operator *= (const double& p)
	{
		*this = this->operator*(p);
		return *this;
	}

	const price_t operator / (const double& p) const // in-scale division
	{
		price_t res(*this);
		unsigned short s(res.m_scale);
		res.assign(res.get_double() / p);
		res.change_scale(s);

		return res;
	}

	price_t& operator /= (const double& p)
	{
		*this = this->operator/(p);
		return *this;
	}

	// logic operators
	bool operator > (const price_t& p) const
	{
		if (get_integral() <= p.get_integral())
		{
			if (m_scale <= p.m_scale)
			{
				return (get_fractional(p.m_scale - m_scale) > p.get_fractional());
			}
			else
			{
				return (get_fractional() > p.get_fractional(m_scale - p.m_scale));
			}
		}

		return true;
	}

	bool operator >= (const price_t& p) const
	{
		if (get_integral() < p.get_integral())
		{
			if (m_scale <= p.m_scale)
			{
				return (get_fractional(p.m_scale - m_scale) >= p.get_fractional());
			}
			else
			{
				return (get_fractional() >= p.get_fractional(m_scale - p.m_scale));
			}
		}

		return true;
	}

	bool operator < (const price_t& p) const
	{
		return !this->operator >=(p);
	}

	bool operator <= (const price_t& p) const
	{
		return !this->operator > (p);
	}

	bool operator == (const price_t& p) const
	{
		if(get_integral() == p.get_integral())
		{
			if (m_scale <= p.m_scale)
			{
				return (get_fractional(p.m_scale - m_scale) == p.get_fractional());
			}
			else
			{
				return (get_fractional() == p.get_fractional(m_scale - p.m_scale));
			}
		}

		return false;
	}

	bool operator != (const price_t& p) const
	{
		return !this->operator==(p);
	}

	bool operator == (const double& d) const
	{
		return this->operator==(price_t(d));
	}

	bool operator != (const double& d) const
	{
		return this->operator==(price_t(d));
	}

	// cast
	long double get_double() const
	{
		return m_price / pow(10, m_scale);
	}

	// other
	void mult(const double& d)
	{
		assign(get_double() * d);
	}

	void div(const double& d)
	{
		assign(get_double() / d);
	}

private:
	void assign(const long double& d)
	{
		m_scale = GetScale(d);
		m_price = static_cast<int64_t>(d * pow(10, m_scale));
	}

	int64_t get_integral() const
	{
		return static_cast<int64_t>(m_price / pow(10, m_scale));
	}

	int64_t get_fractional(unsigned short s = 0) const
	{
		return static_cast<int64_t>(m_price % static_cast<uint64_t>(pow(10, m_scale)) * pow(10, s));
	}

	void change_scale(unsigned short s)
	{
		if (s == m_scale)
			return;

		if (s > m_scale)
		{
			m_price *= pow(10, s - m_scale);
		}
		else
		{
			m_price = std::llround(m_price / pow(10, m_scale - s));
		}

		m_scale = s;
	}

private:
	unsigned short GetScale(long double d, const long double& prec = 1e-6)
	{
		unsigned short s(1);
		while ((d - static_cast<int64_t>(d)) > prec && s++)
			d *= 10;

		return s - 1;
	}
};

template<>
const price_t<managed> price_t<managed>::operator+(const price_t<managed>& p) const
{
	price_t<managed> res(*this);
	res.m_price = managed::checked_addition(res.get_integral(), p.get_integral());
	res.m_price = managed::checked_mult(res.m_price, pow(10, res.m_scale));
	res.m_price = managed::checked_addition(res.m_price, res.m_scale < p.m_scale
		? managed::checked_addition(get_fractional(p.m_scale - res.m_scale), p.get_fractional())
		: managed::checked_addition(get_fractional(), p.get_fractional(res.m_scale - p.m_scale)));

	return res;
}

template<>
const price_t<managed> price_t<managed>::operator-(const price_t<managed>& p) const
{
	price_t<managed> res(*this);
	res.m_price = managed::checked_subtraction(res.get_integral(), p.get_integral());
	res.m_price = managed::checked_mult(res.m_price, pow(10, res.m_scale));
	res.m_price = managed::checked_subtraction(res.m_price, res.m_scale < p.m_scale
		? managed::checked_subtraction(get_fractional(p.m_scale - res.m_scale), p.get_fractional())
		: managed::checked_subtraction(get_fractional(), p.get_fractional(res.m_scale - p.m_scale)));

	return res;
}

template<>
void price_t<managed>::assign(const long double& d)
{
	m_scale = GetScale(d);
	m_price = managed::checked_assign(d * pow(10, m_scale));
}

template<>
void price_t<managed>::change_scale(unsigned short s)
{
	if (s == m_scale)
		return;

	if (s > m_scale)
	{
		m_price = managed::checked_assign(pow(10, s - m_scale) * m_price);
	}
	else
	{
		m_price = std::llround(m_price / pow(10, m_scale - s));
	}

	m_scale = s;
}

typedef price_t<unmanaged> TPrice;
typedef price_t<managed> TPriceManaged;
