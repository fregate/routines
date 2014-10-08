
class fps
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
