#include "common.h"
#include <iostream>
#include <string>
#include <fstream>
#include <queue>
#include <unordered_map>

using ull = unsigned long long;

static std::pair<ull, ull> solve_both_parts(const std::string& filename)
{
	std::priority_queue<int> a;
	std::priority_queue<int> b;
	std::unordered_map<int, int> freq;

	std::ifstream ifs (filename);
	while(ifs) 
	{
		int left{}, right{};
		ifs >> left;
		if(!ifs) break; 
		ifs >> right;
		
		a.push(-left);
		b.push(-right);
		++freq[right];
	}

	unsigned long long sum{};
	unsigned long long part2{};
	while(!a.empty())
	{
		auto first = -a.top();
		auto second = -b.top();
		a.pop();
		b.pop();
		part2 += freq[first] * first;
		sum += std::abs(first - second);
	}
	return {sum, part2};
}

template<>
std::string DaySolver<1>::part1() 
{
	return std::to_string(solve_both_parts(filename).first);
}

template<>
std::string DaySolver<1>::part2() 
{
	return std::to_string(solve_both_parts(filename).second);
}