#include "common.h"

using Colors = std::vector<std::string>;
using Key = std::pair<std::string, int>;
using Cache = std::unordered_map<std::string_view, uint64_t>;

uint64_t possible(Cache& cache, const std::string_view& q, int i, Colors& colors)
{
	auto key = q.substr(i);
	if(cache.count(key))
		return cache[key];

	uint64_t res = 0;
	if (i == q.size())
		return 1;

	for(const auto& clr : colors)
	{
		if(clr.size() > q.size()-i)
			continue;

		bool eq = true;
		for(int j = 0; j < clr.size(); ++j)
		{
			if (q[i + j] != clr[j])
			{
				eq = false;
				break;
			}
		}

		if(eq)
			res += possible(cache, q, i + clr.size(), colors);
		cache[key] = res;
	}
	return res;
}

template<>
std::string DaySolver<19>::part1()
{
	std::ifstream ifs (filename);
	Colors colors;
	int num = -1;
	std::vector<std::string> towels;
	for(std::string line; std::getline(ifs, line);)
	{
		++num;
		if(num == 0)
		{
			colors = split<std::string>(line, ", ");
		}
		else if(num == 1)
		{
			continue;
		}
		else {
			towels.push_back(line);
		}
	}
	uint64_t res = 0;
	num = 0;
	for(const auto& q : towels)
	{
		Cache cache;
		auto r = (possible(cache, q, 0, colors) > 0);
		res += r;
	}

	return std::to_string(res);
	
}

template<>
std::string DaySolver<19>::part2()
{
	std::ifstream ifs (filename);
	Colors colors;
	int num = -1;
	std::vector<std::string> towels;
	for(std::string line; std::getline(ifs, line);)
	{
		++num;
		if(num == 0)
		{
			colors = split<std::string>(line, ", ");
		}
		else if(num == 1)
		{
			continue;
		}
		else {
			towels.push_back(line);
		}
	}

	uint64_t res = 0;
	for(const auto& q : towels)
	{
		Cache cache;
		res += possible(cache, q, 0, colors);
	}
	return std::to_string(res);
}
