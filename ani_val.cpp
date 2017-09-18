
template<class V>
struct easing
{
	virtual double operator()(const V& v1, const V& v2, unsigned short c, unsigned short ss) const abstract;
};

template<class V>
struct linear : public easing<V>
{
	virtual double operator()(const V& v1, const V& v2, unsigned short c, unsigned short ss) const
	{
		return v1 + (v2 - v1) / ss * (c + 1);
	}
};

// class for animated value. step by step
template <class T>
class aniv
{
protected:
	const double epsilon_;
	const unsigned short s_;

	const easing<T>& ef_;

	T v_;
	T sv_;

	double cv_;
	unsigned short cs_;

	bool in_e(const T& nv) const 
	{
		return abs(cv_ - nv) < 2 * epsilon_;
	}

public:
	aniv(const T& val, unsigned short steps, double e, const easing<T>& ef)
		: v_(val)
		, sv_(val)
		, s_(steps)
		, cs_(steps)
		, cv_(val)
		, epsilon_(e)
		, ef_(ef)
	{}

	virtual void step()
	{
		if(cs_ == s_)
		{
			cv_ = v_;
			return;
		}

		cv_ = ef_(sv_, v_, cs_, s_);
		cs_++;
	}

	T get() const
	{
		return (T)cv_;
	}

	bool is_done() const
	{
		return cs_ == s_;
	}

	virtual void set(const T& nv)
	{
		if(in_e(nv))
		{
			sv_ = cv_ = v_ = nv;
			cs_ = s_;
		}
		else
		{
			sv_ = cv_;
			v_ = nv;
			cs_ = 0;
		}
	}
};

// class for cyclic animation. with range and direction
template<class T>
class aniv_c : public aniv<T>
{
protected:
	T v_max, v_min;

	int dir;

	bool is_pivot()
	{
		return false;
	}

	void normalize(T& v)
	{
		if(v > v_max)
			v -= (v_max - v_min);

		if(v < v_min)
			v += (v_max - v_min);
	}

public:
	aniv_c(const T& val, unsigned short steps, double e, const easing<T>& ef, const T& vi, const T& va)
		: aniv(val, steps, e, ef)
		, v_max(va)
		, v_min(vi)
		, dir(1)
	{}

	virtual void step()
	{
		if(cs_ == s_)
		{
			cv_ = v_;
			return;
		}

		// use dir
		if(dir > 0)
		{
			if(sv_ < v_)
			{
				cv_ = ef_(sv_, v_, cs_, s_);
			}
			else
			{
				cv_ = ef_(sv_, v_ + (v_max - v_min), cs_, s_);
				normalize(cv_);
			}
		}
		else
		{
			if(sv_ > v_)
			{
				cv_ = ef_(sv_, v_, cs_, s_);
			}
			else
			{
				cv_ = ef_(sv_, v_ - (v_max - v_min), cs_, s_);
				normalize(cv_);
			}		
		}

		cs_++;
	}

	virtual void set(const T& nv)
	{
		if(nv < v_ && dir > 0 && in_e(nv))
		{
			sv_ = cv_ = v_ = nv;
			cs_ = s_;
			return;
		}

		if(nv > v_ && dir < 0 && in_e(nv))
		{
			sv_ = cv_ = v_ = nv;
			cs_ = s_;
			return;
		}

		sv_ = cv_;
		v_ = nv;
		cs_ = 0;
	}

	void set_direction(int d)
	{
		dir = d;
	}
};
