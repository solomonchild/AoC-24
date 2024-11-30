#include "common.h"

#include <fstream>
#include <iostream>
#include <string>
#include <regex>

static inline char peek(const std::string& str, int idx)
{
	if(idx >= str.size())
		return 0xff;
	return str[idx];
}

static inline bool get_digit(const std::string& str, int& idx, int& num)
{
	int start = idx;
	int end = idx;
	while (std::isdigit(peek(str, end))) 
	{
		++end;
	}
	idx = end;
	if (end - start != 0)
	{
		num = std::stoi(str.substr(start, end-start+1));
		return true;
	}
	return false;
}

static std::string solve_pt1_non_regexp(std::ifstream& ifs)
{
	long long res{};
	std::string line;
	while(std::getline(ifs, line))
	{
		int i = 0;
		while(i < line.size())
		{
			if(peek(line, i) == 'm' && peek(line, i+1) == 'u' && peek(line, i+2) == 'l' && peek(line, i+3) == '(')
			{
				i += 4;
				int a{};
				if (get_digit(line, i, a) && peek(line, i) == ',')
				{
					++i;
					int b{};
					if(get_digit(line, i, b) && peek(line, i) == ')')
					{
						res += a*b;
					}
				}
			}
			else
				++i;
		}
	}
	return std::to_string(res);
}

static std::string solve_pt1_regexp(std::ifstream& ifs)
{
	static const std::regex mul("mul\\((\\d+),(\\d+)\\)");
	std::string line;
	long long res {};
	while(std::getline(ifs, line))
	{
		auto b = std::sregex_iterator(line.begin(), line.end(), mul);
		auto e = std::sregex_iterator();
		for(auto i = b; i != e; ++i)
		{
			auto a = std::stoi((*i)[1].str());
			auto b = std::stoi((*i)[2].str());
			res += a*b;
		}
	}
	return std::to_string(res);
}

template<>
std::string DaySolver<3>::part1() 
{
	std::ifstream ifs (filename);
#if 1
	return solve_pt1_non_regexp(ifs);
#else
	return solve_pt1_regexp(ifs);
#endif
}

template<>
std::string DaySolver<3>::part2()
{
	static const std::regex mul("(mul\\((\\d+),(\\d+)\\)|do\\(\\)|don't\\(\\))");
	std::string line;
	long long res {};
	bool isdo = true;

	std::ifstream ifs(filename);
	while(std::getline(ifs, line))
	{
		auto b = std::sregex_iterator(line.begin(), line.end(), mul);
		auto e = std::sregex_iterator();
		for(auto i = b; i != e; ++i)
		{
			if(i->str().find("don't") != std::string::npos)
			{
				isdo = false;
			}
			else if(i->str().find("do") != std::string::npos)
			{
				isdo = true;
			}
			else if(i->str().find("mul") != std::string::npos && isdo)
			{
				auto a = std::stoi((*i)[2].str());
				auto b = std::stoi((*i)[3].str());
				res += a*b;
			}
		}
	}
	return std::to_string(res);
}