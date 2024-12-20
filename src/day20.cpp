#include "common.h"
#include <queue>
#include <set>
#include <map>
#include <stack>


using Obstacles = std::unordered_set<Pos, pos_hash>;
using Distances = std::unordered_map<Pos, int, pos_hash>;
std::tuple<Obstacles, Pos, Pos, int, int> parse(std::istream& iss)
{
	Obstacles obsts;
	Pos start;
	Pos end;
	int row = 0;
	int width = 0;
	for(std::string line; std::getline(iss, line);)
	{
		if(line.empty())
			continue;
		if(width == 0)
			width = line.size();

		for (int i = 0; i < line.size(); ++i)
		{
			if (line[i] == '#')
				obsts.insert(Pos{i, row});
			else if (line[i] == 'E')
			{
				end = {i, row};
			}
			else if (line[i] == 'S')
			{
				start = {i, row};
			}
		}
		++row;
	}
	return {obsts, start, end, row, width};
}

bool oob(Pos pos, int height, int width)
{
	return pos.real() <= 0 || pos.imag() <= 0 || pos.real() >= width || pos.imag() >= height;
}

using Path = std::vector<Pos>;
std::pair<int, Path> find_fastest(Obstacles& obsts, int width, int height, Pos start, Pos end)
{
	std::queue<std::tuple<Pos, int, Path>> Q;
	Q.push({start, 0, {}});
	std::unordered_set<Pos, pos_hash> visited;
	int res = INT_MAX;
	Path res_path;

	while(!Q.empty())
	{
		auto [pos, cost, path] = Q.front(); Q.pop();
		path.push_back(pos);
		if(pos == end)
		{
			res_path = path;
			res = cost;
			break;
		}

		if (!visited.insert(pos).second)
		{
			continue;
		}

		for(auto d : NEIGHBOR_DELTAS)
		{
			const auto new_pos = pos + d;
			if (obsts.count(new_pos) || oob(new_pos, height, width))
				continue;

			Q.push({new_pos, cost + 1, path});
		}
	}
	return {res, res_path};
}


int get_cheats_num(Path best_path, const int max, const int DELTA, const int best, Obstacles& obsts, int width, int height, Pos start, Pos end)
{
	std::unordered_map<int, int> freq;

	for (int i = 0; i < best_path.size(); ++i)
	{
		for (int j = i + 1; j < best_path.size(); ++j)
		{
			const auto diff = best_path[j] - best_path[i];

			int man_dist = std::abs(diff.real()) + std::abs(diff.imag());
			if (man_dist > max)
				continue;

			const auto delta = j - i - man_dist;
			if(delta < 0 || delta < DELTA)
				continue;
			++freq[delta];
		}
	}

	int cnt = 0;
	for(auto [k,v] : freq)
	{
		cnt += v;
	}
	return cnt;
}

void test()
{
	{
		std::string t = R"!<(
###############
#...#...#.....#
#.#.#.#.#.###.#
#S#...#.#.#...#
#######.#.#.###
#######.#.#...#
#######.#.###.#
###..E#...#...#
###.#######.###
#...###...#...#
#.#####.#.###.#
#.#...#.#.#...#
#.#.#.#.#.#.###
#...#...#...###
###############)!<";
		std::istringstream oss(t);
		auto [obs, start, end, h, w] = parse(std::move(oss));

		auto [best_score, path] = find_fastest(obs, w, h, start, end);
		auto res = get_cheats_num(path, 2, 0, best_score, obs, w, h, start, end);
		assert(res == 44);
	}
	{
		std::string t = R"!<(
###############
#...#...#.....#
#.#.#.#.#.###.#
#S#...#.#.#...#
#######.#.#.###
#######.#.#...#
#######.#.###.#
###..E#...#...#
###.#######.###
#...###...#...#
#.#####.#.###.#
#.#...#.#.#...#
#.#.#.#.#.#.###
#...#...#...###
###############)!<";
		std::istringstream oss(t);
		auto [obs, start, end, h, w] = parse(std::move(oss));

		auto [best_score, path] = find_fastest(obs, w, h, start, end);
		auto res = get_cheats_num(path, 20, 50, best_score, obs, w, h, start, end);
		assert(res == 285);
	}
}
template<>
std::string DaySolver<20>::part1()
{
	//test();
	auto [obsts, start, end, height, width] = parse(std::ifstream(filename));
	auto [best_score,path] = find_fastest(obsts, width, height, start, end);
	auto res = get_cheats_num(path, 2, 100, best_score, obsts, width, height, start, end);
	return std::to_string(res);
}

template<>
std::string DaySolver<20>::part2()
{
	//test();
	auto [obsts, start, end, height, width] = parse(std::ifstream(filename));
	auto [best_score,path] = find_fastest(obsts, width, height, start, end);
	auto res = get_cheats_num(path, 20, 100, best_score, obsts, width, height, start, end);
	return std::to_string(res);
}