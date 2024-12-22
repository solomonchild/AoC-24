#include "common.h"

#include <numeric>

std::vector<i64> parse_numbers(std::ifstream ifs)
{
	std::vector<i64> numbers;
	for(std::string line; std::getline(ifs, line);)
	{
		numbers.push_back(std::stoi(line));
	}
	return numbers;
}

template <>
std::string DaySolver<22>::part1()
{
	auto numbers = parse_numbers(std::ifstream(filename));

	for (int ni = 0; ni < numbers.size(); ++ni)
	{
		auto &num = numbers[ni];
		for (int i = 0; i < 2000; ++i)
		{
			num = (num ^ num * 64) % 16777216;
			num = (num ^ num / 32) % 16777216;
			num = (num ^ num * 2048) % 16777216;
		}
	}
	return std::to_string(std::accumulate(numbers.begin(), numbers.end(), i64(0)));
}

template <>
std::string DaySolver<22>::part2()
{
	std::vector<i64> numbers = parse_numbers(std::ifstream(filename));

	umap<int, int> total_price_by_quad;
	for (int ni = 0; ni < numbers.size(); ++ni)
	{
		auto &num = numbers[ni];
		uset<int> seen;
		int diffs = 0;
		for (int i = 0; i < 1999; ++i)
		{

			const int prev_price = num % 10;
			num = (num ^ num * 64) % 16777216;
			num = (num ^ num / 32) % 16777216;
			num = (num ^ num * 2048) % 16777216;
			const int price = num % 10;
			const auto diff = price - prev_price;

			diffs = ((diffs << 5) | (diff < 0 ? -diff + 10 : diff)) & 0x0fffff;

			if (i >= 3 && price)
			{
				if (seen.insert(diffs).second)
				{
					total_price_by_quad[diffs] += price;
				}
			}
		}
	}
	auto m = std::max_element(total_price_by_quad.begin(), total_price_by_quad.end(), [](const Pair<int, int>& a, const Pair<int, int>& b){
		return a.second < b.second;
	});
	return std::to_string(m->second);
}
