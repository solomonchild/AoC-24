#include "common.h"

#include <functional>
#include <stack>

using Path = std::vector<Vec2>;
Path dfs(Vec2 start, Vec2 end, std::function<bool(const Vec2&)> should_add_neighbor)
{
	std::stack<Pair<Vec2, Path>> Q;
	Q.push({start, {}});
	while(!Q.empty())
	{
		auto [pos, path] = Q.top(); Q.pop();
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

Path djikstra(Vec2 start, Vec2 end, std::function<bool(const Vec2&)> should_add_neighbor)
{
	struct Elem
	{
		Vec2 pos;
		int dist;
		Path path;
	};
	auto cmp = [](const Elem& a, const Elem& b){
		return a.dist > b.dist;
	};
	std::priority_queue<Elem, std::vector<Elem>, decltype(cmp)> Q(cmp);
	Q.push({start});
	while(!Q.empty())
	{
		auto [pos, dist, path] = Q.top(); Q.pop();
		path.push_back(pos);

		if (pos == end)
		{
			return path;
		}

		for(auto d : NEIGHBOUR_DELTAS)
		{
			if (should_add_neighbor(pos+d))
			{
				Q.push({pos+d, dist+1, path});
			}
		}
	}
	return {};
}


template <>
std::string DaySolver<25>::part1()
{
	std::ifstream ifs(filename);

	for(std::string line; std::getline(ifs, line);)
	{
	}
	return std::to_string(0);
}


template <>
std::string DaySolver<25>::part2()
{
	return "";
}
