#include "common.h"

#include <sstream>
#include <queue>

std::tuple<Grid, Pos, Pos> parse(std::istream&& iss)
{
	Grid grid;
	Pos start;
	Pos end;
	for(std::string line; std::getline(iss, line);)
	{
		if(line.empty())
			continue;
		grid.push_back(line);
		if(auto i = line.find("E"); i != std::string::npos)
		{
			end = {int(i), int(grid.size()-1)};
		}
		if(auto i = line.find("S"); i != std::string::npos)
		{
			start = {int(i), int(grid.size()-1)};
		}
	}
	return {grid, start, end};
}

inline int get_turns(Pos a, Pos b)
{
	//first time visited
	if (a == Pos{0, 0})
		return 0;

	int cost1 = 0;
	int cost2 = 0;
	Pos t = a;
	while(t != b)
	{
		t *= {0, -1};
		++cost1;
	}
	t = a;
	while(t != b)
	{
		t *= {0, +1};
		++cost2;
	}
	return std::min(cost2, cost1);
}



std::pair<int, int> djikstra(Grid& grid, Pos start, Pos end)
{
	std::unordered_set<std::pair<Pos, Dir>, pos_dir_hash> visited;
	std::unordered_map<Pos, int, pos_hash> dist;

	std::queue<std::tuple<Pos, Dir, int, std::vector<Pos>>> Q;
	Q.push({start, {1, 0}, 0, {}});
	int best_cost = INT_MAX;
	std::unordered_map<Pos, int, pos_hash> best;

	while(!Q.empty())
	{
		auto [pos, dir, cost, path] = Q.front(); Q.pop();
		if(cost > best_cost)
			continue;

		path.push_back(pos);
		if(pos == end)
		{
			if(cost < best_cost)
			{
				best_cost = cost;
				best.clear();
			}
			if(cost == best_cost)
			{
				for(auto i : path)
				{
					++best[i];
				}
			}
			continue;
		}

		if (!visited.insert({pos, dir}).second && cost -1000 > dist[pos])
		{
			continue;
		}
		else if(!dist.count(pos) || cost <= dist[pos])
		{
			dist[pos] = cost;
		}
		for(auto d : NEIGHBOR_DELTAS)
		{
			auto new_pos = pos+d;
			if (peek(grid, new_pos) == '#')
				continue;

			const int new_cost = cost + 1 + get_turns(dir, d)*1000;
			Q.push({new_pos, d, new_cost, path});
		}
	}
	int res = int(best.size());
	DLOG("%d", res);
	return {best_cost, res};
}

static void test()
{

	assert(get_turns({0,1}, {0,1}) == 0);
	assert(get_turns({0,1}, {1,0}) == 1);
	assert(get_turns({0,1}, {0,-1}) == 2);
	assert(get_turns({0,1}, {-1,0}) == 1);

	assert(get_turns({1,0}, {1,0}) == 0);
	assert(get_turns({1,0}, {0,-1}) == 1);
	assert(get_turns({1,0}, {0,1}) == 1);
	assert(get_turns({1,0}, {-1,0}) == 2);

	assert(get_turns({0,-1}, {0,-1}) == 0);
	assert(get_turns({0,-1}, {1,0}) == 1);
	assert(get_turns({0,-1}, {-1,0}) == 1);
	assert(get_turns({0,-1}, {0,1}) == 2);

	assert(get_turns({-1,0}, {-1,0}) == 0);
	assert(get_turns({-1,0}, {1,0}) == 2);
	assert(get_turns({-1,0}, {0,-1}) == 1);
	assert(get_turns({-1,0}, {0,1}) == 1);
	{
		std::string input = R"!(
#######
#...#E#
#S....#
#######)!";
		DLOG("-----------TEST-------------");
		std::stringstream iss(input);
		auto [grid, start, end] = parse(std::move(iss));
		auto [res,_] = djikstra(grid, start, end);
		DLOG("%d", res);
		assert( res == 1005);
	}

	{
		std::string input = R"!(
###############
#.......#....E#
#S............#
###############)!";
		DLOG("-----------TEST-------------");
		std::stringstream iss(input);
		auto [grid, start, end] = parse(std::move(iss));
		auto [res, _] = djikstra(grid, start, end);
		DLOG("%d", res);
		assert( res == 1013);
	}
	{
		std::string input = R"!(
###############
#.......#....E#
#.....#...#.#.#
#.###.#.#.#.#.#
#S..#.....#...#
###############)!";
		std::stringstream iss(input);
		auto [grid, start, end] = parse(std::move(iss));
		auto [res, _] = djikstra(grid, start, end);
		DLOG("Res: %d", res);
		print(grid);
		assert(res == 6017);
	}

	{
		std::string input = R"!(
#################
#...#...#...#..E#
#.#.#.#.#.#.#.#.#
#.#.#.#...#...#.#
#.#.#.#.###.#.#.#
#...#.#.#.....#.#
#.#.#.#.#.#####.#
#.#...#.#.#.....#
#.#.#####.#.###.#
#.#.#.......#...#
#.#.###.#####.###
#.#.#...#.....#.#
#.#.#.#####.###.#
#.#.#.........#.#
#.#.#.#########.#
#S#.............#
#################)!";
		std::stringstream iss(input);

		DLOG("-----------TEST-------------");
		auto [grid, start, end] = parse(std::move(iss));
		auto [res, nodes] = djikstra(grid, start, end);
		DLOG("Res: %d", res);
		print(grid);
		assert(nodes == 64);
		assert(res == 11048);
	}
	{
		std::string input = R"!(
###############
#.......#....E#
#.#.###.#.###.#
#.....#.#...#.#
#.###.#####.#.#
#.#.#.......#.#
#.#.#####.###.#
#...........#.#
###.#.#####.#.#
#...#.....#.#.#
#.#.#.###.#.#.#
#.....#...#.#.#
#.###.#.#.#.#.#
#S..#.....#...#
###############)!";
		std::stringstream iss(input);

		DLOG("-----------TEST-------------");
		auto [grid, start, end] = parse(std::move(iss));
		auto [res, nodes] = djikstra(grid, start, end);
		DLOG("Res: %d", res);
		print(grid);
		assert(nodes == 45);
		assert(res == 7036);
	}
};

template<>
std::string DaySolver<16>::part1()
{
	//test();
	auto [grid, start, end] = parse(std::ifstream(filename));
	auto [res, nodes] = djikstra(grid, start, end);
	return std::to_string(res);
}

template<>
std::string DaySolver<16>::part2()
{
	//test();
	auto [grid, start, end] = parse(std::ifstream(filename));
	auto [res, nodes] = djikstra(grid, start, end);
	return std::to_string(nodes);
}
