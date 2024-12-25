#include "common.h"

bool is_lock(Strings& lock_key)
{
	for (auto c : lock_key[0])
	{
		if (c == '#')
			return true;
	}
	return false;
}

template <>
std::string DaySolver<25>::part1()
{
	std::ifstream ifs(filename);

	std::vector<Ints> keys;
	std::vector<Ints> locks;
	Strings lock_key;

	auto process = [&]() -> int
	{
		const bool lock = is_lock(lock_key);
		std::vector<int> &cur = lock ? (locks.push_back({}), locks.back()) : (keys.push_back({}), keys.back());

		for (int j = 0; j < lock_key[0].size(); ++j)
		{
			int height = 0;
			for (int i = 0; i < lock_key.size(); ++i)
			{
				if (lock_key[i][j] == '#')
					++height;
			}
			cur.push_back(height);
		}
		const int height = lock_key.size();
		lock_key.clear();
		return height;
	};

	for(std::string line; std::getline(ifs, line);)
	{
		if(line.empty())
		{
			process();
			continue;
		}
		lock_key.push_back(line);
	}
	const int height = process();

	auto overlap = [&](const std::vector<int>& k, const std::vector<int>& l)
	{
		assert(l.size() == k.size());
		for(int i = 0; i < l.size(); ++i)
		{
			if(k[i] + l[i] > height)
			return false;
		}
		return true;
	};

	int res = 0;
	for(const auto& k : keys)
	{
		for (const auto& l : locks)
		{
			if(overlap(k, l))
			++res;
		}
	}
	return std::to_string(res);
}

template <>
std::string DaySolver<25>::part2()
{
	return "";
}
