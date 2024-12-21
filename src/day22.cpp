#include "common.h"

#include <functional>

using Path = std::vector<Vec2>;
Path bfs(Vec2 start, Vec2 end, std::function<bool(const Vec2&)> should_add_neighbor)
{
	std::queue<Pair<Vec2, Path>> Q;
	Q.push({start, {}});
	while(!Q.empty())
	{
		auto [pos, path] = Q.front(); Q.pop();
		path.push_back(pos);

		if (pos == end)
		{
			return path;
		}

		for(auto d : NEIGHBOUR_DELTAS)
		{
			if (should_add_neighbor(pos+d))
			{
				Q.push({pos+d, path});
			}
		}
	}
	return {};
}

template <>
std::string DaySolver<22>::part1()
{
	std::ifstream ifs(filename);
	for(std::string line; std::getline(ifs, line);)
	{
	}
	return "";
}

template <>
std::string DaySolver<22>::part2()
{
	return "";
}