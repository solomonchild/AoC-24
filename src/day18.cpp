#include "common.h"
#include <queue>

int bfs(Pos start, Pos end, std::unordered_set<Pos, pos_hash> &unsafe)
{
	std::unordered_set<Pos, pos_hash> seen;
	std::queue<std::pair<Pos, int>> Q;

	int best = INT_MAX;

	Q.push({start, 0});
	while (!Q.empty())
	{
		auto [pos, dist] = Q.front(); Q.pop();

		if (!seen.insert(pos).second)
			continue;

		if (pos == end)
		{
			best = std::min(best, dist);
		}

		for (auto n : NEIGHBOR_DELTAS)
		{
			auto new_pos = pos + n;
			if (!unsafe.count(pos + n) && new_pos.real() >= 0 && new_pos.imag() >= 0 && new_pos.real() <= end.real() && new_pos.imag() <= end.imag())
				Q.push({new_pos, 1 + dist});
		}
	}

	return best;
}

std::pair<std::vector<Pos>, Pos> parse(std::ifstream ifs)
{
	std::vector<Pos> positions;
	int max_y = 0;
	while(ifs)
	{
		int x = 0;
		int y = 0;
		ifs >> x; ifs.ignore(); ifs >> y;

		max_y = std::max(max_y, y);
		positions.push_back(Pos{x, y});
	}
	Pos end = {70, max_y};
	return {positions, end};
}
template<>
std::string DaySolver<18>::part1()
{
	const Pos start = {0, 0};
	auto [positions, end] = parse(std::ifstream(filename));

	std::unordered_set<Pos, pos_hash> unsafe;
	for(int j = 0; j < 1024; ++j)
		unsafe.insert(positions[j]);

	int res = bfs(start, end, unsafe);
	return std::to_string(res);
}

inline std::string to_string(const Pos& pos)
{
		return std::to_string(pos.real()) + "," + std::to_string(pos.imag());
}

template<>
std::string DaySolver<18>::part2()
{
	const Pos start = {0, 0};
	auto [positions, end] = parse(std::ifstream(filename));
	auto path_exists = [start, end](const std::vector<Pos> &positions, int idx) -> bool {
		std::unordered_set<Pos, pos_hash> unsafe;
		for (auto i = 0; i <= idx; ++i)
			unsafe.insert(positions[i]);
		return bfs(start, end, unsafe) != INT_MAX;
	};

	int l = 0;
	int r = int(positions.size());
	while(l < r)
	{
		const int mid = (l+r)/2;
		if(!path_exists(positions, mid))
			r = mid;
		else
			l = mid+1;
	}

	const int mid = (l + r) / 2;
	if(!path_exists(positions, mid))
		return to_string(positions[mid]);

	if(!path_exists(positions, l))
		return to_string(positions[l]);

	return std::to_string(0);
}
