#include "common.h"

#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

template<>
std::string DaySolver<2>::part1() 
{

	std::ifstream ifs (filename);
	int safe_reports = 0;
	bool safe = true;
	bool dec = true;
	bool first = true;
	int a{}, b{};
	while(ifs) 
	{
		if(ifs.eof())
		{
			if(safe)
			++safe_reports;
			break;
		}
		if(ifs.peek() == '\n' || ifs.peek() == '\r')
		{
			if (safe)
			{
				++safe_reports;
			}
			safe = true;
			dec = true;
			first = true;
		}
		if(first)
		{
			ifs >> a >> b;
			dec = a > b;
			first = false;
		}
		else
		{
			ifs >> b;
		}
		if(safe)
		{
			if (a == b)
			{
				safe = false;
			}
			if (dec != (a > b))
			{
				safe = false;
			}
			if (auto c = std::abs(a - b); c > 3)
			{
				safe = false;
			}
		}
		a = b;
	}

	return std::to_string(safe_reports);
}

std::vector<int> split(const std::string& str)
{
	int i = 0; 
	std::vector<int> ret;
	for(int j = str.find(" ", i); j != std::string::npos; j = str.find(" ", i)) 
	{
		while(j < str.size() && str[j] == ' ')
			++j;
		if (j < str.size())
			ret.push_back(std::stoi(str.substr(i, j - i)));
		i = j;
	}
	while(i < str.size() && str[i] == ' ')
		++i;
	if (i < str.size())
		ret.push_back(std::stoi(str.substr(i)));
	return ret;
}

std::vector<int> nums;
inline bool abs_good(int a, int b)
{
	if(!(a >= 0 && a < nums.size() && b >= 0 && b < nums.size()))
		return true;
	return  std::abs(nums[a] - nums[b]) <= 3;
}

inline bool dec_good(int a, int b, bool dec)
{
	if(!(a >= 0 && a < nums.size() && b >= 0 && b < nums.size()))
		return true;
	return dec == nums[a] > nums[b];
}

inline bool eq_good(int a, int b)
{
	if(!(a >= 0 && a < nums.size() && b >= 0 && b < nums.size()))
		return true;
	return nums[a] != nums[b];
}

inline bool good(int a, int b, bool dec)
{
	if(!(a >= 0 && a < nums.size() && b >= 0 && b < nums.size()))
		return true;
	return eq_good(a, b) && dec_good(a, b, dec) && abs_good(a, b);
}

int report = 1;
bool validate(const std::vector<int>& nums, int start, bool saved = false)
{
	bool dec = false;
	bool safe = true;
	bool first = true;

	auto skip = [&nums, &dec](int a, int b, int skipping, int& i, bool increment = false)
	{
		if (skipping - 2 >=0)
			dec = nums[skipping - 2] > nums[skipping - 1];
		else
			dec = nums[a] > nums[b];
		if (good(a, b, dec) && good(b, b+1, dec))
		{
			DLOG("Skipping %d(%d), turning dec=%d", skipping, nums[skipping], dec);
			if (increment)
				++i;
			return true;
		}
		return false;
	};

	for (int i = 0; i < nums.size() - 1; ++i)
	{
		if(first && nums[i] != nums[i+1])
		{
			dec = nums[i] > nums[i+1];
			first = false;
		}

		if (nums[i] == nums[i + 1])
		{
			DLOG("Report %d, same val %d %d", report, nums[i], nums[i + 1]);
			if (saved)
			{
				safe = false;
				break;
			}
			saved = true;
			continue;
		}
		else if (!dec_good(i, i + 1, dec) || !abs_good(i, i + 1))
		{
			DLOG("Report %d, bad dec/abs %d %d", report, nums[i], nums[i + 1]);
			if (!saved)
			{
				saved = true;
				if (i == 0)
				{
					if (skip(1, 2, 0, i))
						continue;
					else if (skip(0, 2, 1, i, true))
						continue;
				}
				else if (i == 1)
				{
					if (skip(1, 2, 0, i))
						continue;
					else if (skip(0, 2, 1, i))
						continue;
					else if (skip(0, 1, 2, i, true))
						continue;
				}
				else
				{
					if (skip(i - 1, i + 1, i, i))
						continue;
					else if (skip(i, i + 2, i + 1, i, true))
						continue;
				}
			}

			safe = false;
			break;
		}
	}
	return safe;
}

template<>
std::string DaySolver<2>::part2()
{

	std::ifstream ifs(filename);
	std::string line;
	int safe_reports = 0;
	while (std::getline(ifs, line))
	{
		while(line[0] == ' ')
			line.erase(line.begin());
		if(line[0] == '#' || line.empty())
			continue;
		nums = split(line);
		DLOG("\n%d) %s", report, line.c_str());
		bool safe = validate(nums, 0);
		if (safe)
		{
			DLOG("Report %d safe", report);
			++safe_reports;
		} 
		else
		{
			DLOG("Report %d UNSAFE", report);
		}
		++report;
	}
	return std::to_string(safe_reports);
}