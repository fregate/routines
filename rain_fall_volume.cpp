
#include <vector>
#include <algorithm>

void get_input(std::vector<unsigned int>& vi) // interface for input data
{
	vi.push_back(5);
	vi.push_back(1);
	vi.push_back(1);
	vi.push_back(0);
	vi.push_back(1);
	vi.push_back(5);
}

template<class TInt>
TInt check_val(TInt vcurrent, TInt vidx, TInt& vmax)
{
	if(vmax < vidx)
	{
		vmax = vidx;
		return 0;
	}
	else
	{
		return (vcurrent < (vmax - vidx)) ? vcurrent : (vmax - vidx);
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	std::vector<unsigned int> vinput;
	get_input(vinput);

	if(vinput.empty() || vinput.size() == 1)
	{
		// volume = 0
		return 0;
	}

	size_t vis = vinput.size();
	size_t idxfwd(0), idxbwd(vis - 1);

	std::vector<unsigned int> output;
	output.resize(vis, UINT_MAX);

	unsigned int mvalfwd = vinput[idxfwd];
	unsigned int mvalbwd = vinput[idxbwd];

	while(idxbwd)
	{
		// check forward iterators
		output[idxfwd] = check_val(output[idxfwd], vinput[idxfwd], mvalfwd);
		idxfwd++;

		// check backward iterator
		output[idxbwd] = check_val(output[idxbwd], vinput[idxbwd], mvalbwd);
		idxbwd--;
	}

	// volume = sum of all elements in output
	return 0;
}
