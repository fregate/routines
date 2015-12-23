
#include "stdafx.h"

#include <iostream>
#include <fstream>

#include <string>
#include <vector>
#include <set>
#include <map>

#include <algorithm>
#include <random>

#include <boost/algorithm/string.hpp>

struct Person
{
	int id;
	std::string strName;
	std::string strE;
	bool assigned;

	Person()
		: assigned(false)
	{}
};

typedef std::vector<Person> VecPersons;
typedef std::vector< std::pair<Person, Person> > VecPairs;

class Product
{
	void RebuildRules()
	{
		size_t vsize = vPersons.size();
		if (vsize == 0)
			return;

		bool* pNewRules = new bool[vsize * vsize];
		memset(pNewRules, true, vsize * vsize);

		if (nullptr == pbRules)
		{
			pbRules = pNewRules;
			return;
		}

		for (size_t x = 0; x < vsize - 1; x++)
		{
			for (size_t y = 0; y < vsize - 1; y++)
			{
				bool v = *(pbRules + x * (vsize - 1) + y);
				*(pNewRules + x * vsize + y) = v;
			}
		}

 		delete[] pbRules;
		pbRules = pNewRules;
	}

public:
	VecPersons vPersons;

	bool* pbRules;

	Product()
		: pbRules(nullptr)
	{}

	~Product()
	{
		delete[] pbRules;
	}

	void PrintRules()
	{
		if (nullptr == pbRules)
			return;

		for (size_t x = 0; x < vPersons.size(); x++)
		{
			std::cout << x << "\t";
			for (size_t o = 0; o < vPersons.size(); o++)
			{
				std::cout << (*(pbRules + x * vPersons.size() + o) ? " + " : "   ");
			}

			std::cout << std::endl;
		}
	}

	void AddPerson()
	{
		Person p;
		std::cout << "enter name" << std::endl;
		std::cin >> p.strName;

		AddPersonImpl(p);
	}

	void AddPersonImpl(Person p)
	{
		vPersons.push_back(p);

		RebuildRules();
	}

	void AddRule(size_t idx1, size_t idx2, bool	bCan)
	{
		if (idx1 >= vPersons.size() || idx2 >= vPersons.size())
		{
			assert(false);
			return;
		}

		*(pbRules + idx1 * vPersons.size() + idx2) = bCan;
	}

	template<class C>
	void ShuffleImpl(C& c)
	{
		time_t ttt;
		time(&ttt);
		std::mt19937 engine1(ttt);
		std::shuffle(c.begin(), c.end(), engine1);
	}

	void Shuffle(std::vector<size_t>& res)
	{
		ShuffleImpl(vPersons);

		res.clear();
		res.resize(vPersons.size());
		for (size_t x = 0; x < vPersons.size(); x++)
		{
			res[vPersons[x].id] = x;
		}
	}

	bool Generate(VecPairs& res)
	{
		res.clear();
		if (vPersons.empty() || vPersons.size() < 3)
			return false;

		std::vector<size_t> ps, resid;
		ps.resize(vPersons.size(), 0);

		size_t i(0);
		while (true)
		{
			std::vector<size_t> vIdx;
			for (size_t y = 0; y < vPersons.size(); y++)
			{
				if (*(pbRules + i * vPersons.size() + y) == true && find(resid.begin(), resid.end(), y) == resid.end())
					vIdx.push_back(y);
			}

			if (ps[i] >= vIdx.size())
			{
				if (i == 0)
				{
					break;
				}

				ps[i] = 0;
				resid.pop_back();
				i--;
				continue;
			}

			// giftToIds.push(list[ps[i]]);
			resid.push_back(vIdx[ps[i]]);
			ps[i]++;
			i++;

			if (vPersons.size() == i)
			{
				break;
			}
		}

		if (resid.empty())
			return false;

		for (i = 0; i < vPersons.size(); i++)
		{
			res.push_back(std::make_pair(vPersons[i], vPersons[resid[i]]));
		}

		return true;
	}

	bool RecurseGeneration(size_t next, std::vector<size_t>& res)
	{
		std::vector<size_t> vIdx;
		for (size_t y = 0; y < vPersons.size(); y++)
		{
			if (*(pbRules + next * vPersons.size() + y) == true && find(res.begin(), res.end(), y) == res.end())
				vIdx.push_back(y);
		}

		if (vIdx.empty())
		{
			if (res.size() != vPersons.size() - 1)
			{
				return false;
			}
			else
			{
				res.push_back(next);
				return true;
			}
		}

		for (size_t x = 0; x < vIdx.size(); x++)
		{
			res.push_back(next);
			if (RecurseGeneration(vIdx[x], res))
			{
				return true;
			}
			else
			{
				res.pop_back();
			}
		}

		return false;
	}

	bool Generate1(VecPairs& res)
	{
		res.clear();
		if (vPersons.empty() || vPersons.size() < 3)
			return false;

		std::vector<size_t> resid;
		RecurseGeneration(0, resid);

		if (resid.empty() || resid.size() != vPersons.size())
			return false;

		for (size_t i = 0; i < vPersons.size() - 1; i++)
		{
			res.push_back(std::make_pair(vPersons[resid[i]], vPersons[resid[i + 1]]));
		}

		res.push_back(std::make_pair(vPersons[*resid.rbegin()], vPersons[resid[0]]));

		return true;
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	Product sss;

	// read data from file
	std::ifstream inf("ppp.txt");
	if (inf.bad())
		return 0;

	bool bRules(false);
	std::string line;
	std::vector<size_t> IDX;
	while (std::getline(inf, line))
	{
		if (line.empty())
		{
			bRules = true;
			sss.Shuffle(IDX);
			continue;
		}

		if (!bRules)
		{
			std::vector<std::string> vstr;
			boost::split(vstr, line, boost::is_any_of("\\"));

			assert(vstr.size() == 3);

			Person p;
			p.strName = boost::trim_copy(vstr[1]);
			p.strE = boost::trim_copy(vstr[2]);
			p.id = atoi(vstr[0].c_str()) - 1;

			sss.AddPersonImpl(p);
		}
		else
		{
			std::vector<std::string> vstr;
			boost::split(vstr, line, boost::is_any_of(" "));

			assert(vstr.size() == 2);

			size_t idx = IDX[atoi(vstr[0].c_str()) - 1];

			for (size_t x = 0; x < vstr[1].size(); x++)
			{
				sss.AddRule(idx, IDX[x], (vstr[1][x] == '+'));
			}
		}
	}

	sss.PrintRules();

	VecPairs res;
 	if (!sss.Generate(res))
 	{
 		std::cout << "Fault! Can't create correct party! Change rules or add more people!" << std::endl;
 	}
 	else
 	{
 		std::ofstream outf("out.txt");
 		for (auto it = res.begin(); it != res.end(); it++)
 		{
 			outf << it->first.strName << " -> " << it->second.strName << std::endl;
 		}
 	}

	if (!sss.Generate1(res)) // create circle
	{
		std::cout << "Fault! Can't create correct party! Change rules or add more people!" << std::endl;
	}
	else
	{
		std::ofstream outf("out2.txt");
		for (auto it = res.begin(); it != res.end(); it++)
		{
			outf << it->first.strName << " -> " << it->second.strName << std::endl;
		}
	}

	return 0;
}


/* file ppp.txt
first block - participiants (num\name\email).
second block (after empty line) - rules (num rulesinfo). rulesinfo: + = allow; - = deny.

// ---- start file --------
01\AA\aa@aa.com
02\BB\bb@bb.com
03\CC\cc@cc.com
04\DD\dd@dd.com

01 -+++
02 +-++
03 ++--
04 ++--
// ------ end file ----- (without empty line)
*/
