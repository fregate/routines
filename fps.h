
class fps_integer // exact
{
	unsigned int m_lastupdate;
	unsigned int m_current;

protected:
	unsigned short m_fps;
	unsigned short m_framecount; // be real - no way to render with more than 65536 frames per second
	unsigned short m_interval;

public:
	// Constructor
	fps_integer(unsigned int interval_s)
		: m_fps(0)
		, m_interval(1000 * interval_s) // convert to ms
		, m_lastupdate(0)
		, m_framecount(0)
	{
	}

	// Update
	void update()
	{
		// increase the counter by one
		m_framecount++;

		m_current = ::GetTickCount();
		if (m_current - m_lastupdate > m_interval)
		{
			m_fps = m_framecount;
			// save the current counter value to m_fps
			m_lastupdate = m_current;
			m_framecount = 0;
		}
	}

	// Get fps
	unsigned short get() const
	{
		return m_fps;
	}
};

class fps // asymptotic integration
{
	unsigned int m_lastupdate;
	unsigned int m_current;

protected:
	unsigned __int64 m_fps;
	unsigned __int64 m_framecount;
	unsigned int m_interval;

public:
	// Constructor
	fps(unsigned int interval_s)
		: m_fps(0)
		, m_interval(1000 * interval_s) // convert to ms
		, m_lastupdate(0)
		, m_framecount(1)
	{
	}

	// Update
	void update()
	{
		// increase the counter by one
		m_fps++;

		m_current = ::GetTickCount();
		if (m_current - m_lastupdate > m_interval)
		{
			m_framecount++;
			// save the current counter value to m_fps
			m_lastupdate = m_current;
		}
	}

	// Get fps
	double get() const
	{
		return ((double) m_fps) / m_framecount;
	}
};
