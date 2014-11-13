
// happy tickets dispersion and distribution

bool all_tickets[1000000] = { false };

unsigned int digits_sum(int num)
{
	int h = num / 100;
	int d = num % 100 / 10;
	int n = num % 10;

	return h + d + n;
}

int _tmain(int argc, _TCHAR* argv[])
{
	for(int h = 0; h < 1000; h++)
	{
		for(int l = 0; l < 1000; l++)
		{
			if(digits_sum(h) == digits_sum(l))
				all_tickets[h * 1000 + l] = true;
		}
	}

	std::map<int, int> frmap;
	int npos(0), lpos(0);
	for(int indx = 0; indx < 1000000; indx++)
	{
		if(all_tickets[indx])
		{
			int d(npos - lpos);
			frmap.insert(std::make_pair(d, 0));
			frmap[d]++;
			lpos = npos;
		}
		npos++;
	}

	std::for_each(frmap.begin(), frmap.end(), [](const std::pair<int, int>& x) {
		std::ostringstream ostrs;
		ostrs << x.first << ";" << x.second << std::endl;
		OutputDebugString(ostrs.str().c_str());
	});
}
