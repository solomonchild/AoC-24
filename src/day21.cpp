#include "common.h"

static umap<char, Vec2> DirCoords =
	{
		{'.', {0, 0}},
		{'^', {1, 0}},
		{'A', {2, 0}},

		{'<', {0, 1}},
		{'v', {1, 1}},
		{'>', {2, 1}},
};

static umap<char, Vec2> NumCoords{
	{'7', {0, 0}},
	{'8', {1, 0}},
	{'9', {2, 0}},

	{'4', {0, 1}},
	{'5', {1, 1}},
	{'6', {2, 1}},

	{'1', {0, 2}},
	{'2', {1, 2}},
	{'3', {2, 2}},

	{'.', {0, 3}},
	{'0', {1, 3}},
	{'A', {2, 3}},
};

static inline int to_num(std::string_view sv)
{
	int num = 0;
	{
		int start = 0;
		while (sv[start] == '0')
			++start;

		if (start != sv.size() - 1)
			num = std::stoi(std::string(sv.substr(start, sv.size() - 1)));
	}
	return num;
}

u64 cheapest(Vec2 src, Vec2 dst, int robots, bool numpad = false);
u64 robot_press(const std::string& presses, int robotnum)
{
	static std::map<std::pair<std::string, int>, u64> cache;
	if(robotnum == 0)
		return presses.size();
	if(cache.contains({presses, robotnum}))
		return cache[{presses, robotnum}];

	u64 res{};
	Vec2 src = DirCoords['A'];
	for(auto c : presses)
	{
		auto dst = DirCoords[c];
		res += cheapest(src, dst, robotnum);
		src = dst;
	}
	cache[{presses, robotnum}] = res;
	return res;
}

u64 cheapest(Vec2 src, Vec2 dst, int robots, bool numpad)
{
	const Vec2 banned_key = numpad ? NumCoords['.'] : DirCoords['.'];

	struct Node
	{
		Vec2 pos;
		std::string presses;
	};

	std::queue<Node> Q;
	Q.push({src, ""});
	u64 res {UINT64_MAX};
	while(!Q.empty())
	{
		auto [pos, presses] = Q.front(); Q.pop();
		if(pos == dst)
		{
			res = std::min(res, robot_press(presses + 'A', numpad ? robots : robots-1));
		}
		else if(pos != banned_key)
		{
			static std::tuple<Vec2, char> deltas[]{{{1, 0}, '>'}, {{-1, 0}, '<'}, {{0, 1}, 'v'}, {{0, -1}, '^'}};
			for(auto [d, c] : deltas)
			{
				if(auto next = (dst-pos) * d; next.x > 0 || next.y > 0)
					Q.push({pos+d, presses + c});
			}
		}
	}
	return res;
}

static u64 solve_day21(std::ifstream ifs, int robots)
{
	u64 res{};
	for (std::string line; std::getline(ifs, line);)
	{
		auto src = NumCoords['A'];
		u64 ret = 0;
		for (auto c : line)
		{
			auto dst = NumCoords[c];
			ret += cheapest(src, dst, robots, true);
			src = dst;
		}

		res += u64(to_num(line)) * ret;
	}
	return res;
}

template <>
std::string DaySolver<21>::part1()
{
	return std::to_string(solve_day21(std::ifstream(filename), 2));
}

template <>
std::string DaySolver<21>::part2()
{
	return std::to_string(solve_day21(std::ifstream(filename), 25));
}