#pragma once

#include <algorithm>
#include <array>
#include <cassert>
#include <chrono>
#include <complex>
#include <fstream>
#include <map>
#include <queue>
#include <stdio.h>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

extern bool DEBUG;

using u64 = uint64_t;
using i64 = int64_t;
template<typename T1, typename T2>
using umap = std::unordered_map<T1, T2>;
template<typename T, typename Hasher=std::hash<T>>
using uset = std::unordered_set<T, Hasher>;

using Grid = std::vector<std::string>;
using Strings = std::vector<std::string>;
using Ints = std::vector<int>;
using Pos = std::complex<int>;
using Dir = std::complex<int>;
template<typename T1, typename T2>
using Pair = std::pair<T1, T2>;
template<typename T1, typename T2, typename T3>
using Triple = std::tuple<T1, T2, T3>;

struct Vec2
{
	int x{}, y{};

	auto operator<=>(const Vec2 &) const = default;

	Vec2 operator+(const Vec2 &a) const
	{
		return {x + a.x, y + a.y};
	}

	Vec2 operator-(const Vec2 &a) const
	{
		return {x - a.x, y - a.y};
	}

	Vec2 operator*(const Vec2 &a) const
	{
		return {x * a.x, y * a.y};
	}
	Vec2 rotate90() const
	{
		return {-y, x};
	}

	Vec2 rotate_neg90() const
	{
		return {y, -x};
	}
};

namespace std
{
	template<>
	struct hash<Vec2>
	{
		size_t operator()(const Vec2& v) const noexcept
		{
			return std::hash<int>()(v.x) * 13 ^ std::hash<int>()(v.y);
		}
	};
};

#define FORE(x, xs) for(auto& x : (xs))
#define REP(i, n) for(int i = 0; i < (n); ++i)

#define DLOG(x, ...) \
do { \
if (DEBUG) { \
	fprintf(stderr, x "\n", ##__VA_ARGS__); \
} \
} while (false)

#define LOG(x, ...) \
do { \
	fprintf(stderr, x "\n", ##__VA_ARGS__); \
} while(false)

static inline std::array<Vec2, 4> NEIGHBOUR_DELTAS 
{
		Vec2{-1, 0}, Vec2{1, 0}, Vec2{0, -1}, Vec2{0, 1}
};

static inline std::vector<Pos> NEIGHBOR_DELTAS 
{
		Pos{-1, 0}, Pos{1, 0}, Pos{0, -1}, Pos{0, 1}
};

static inline std::vector<Pos> NEIGHBOR_DIAG_DELTAS 
{
		Pos{-1, -1}, Pos{1, 1}, Pos{1, -1}, Pos{-1, 1}
};

inline void print(const Grid& grid)
{
	if (DEBUG)
	{
		for(const auto& g : grid)
		{
			DLOG("%s", g.c_str());
		}
	}
}

inline void print(const Ints& ints, bool space = true)
{
	if (DEBUG)
	{
		for(const auto& g : ints)
		{
			const char* fmt_line = space ? "%d " : "%d";
			fprintf(stderr, fmt_line, g);
		}
		DLOG("");
	}
}

struct pos_hash {
    inline std::size_t operator()(const Pos& v) const {
        return v.real()*31+v.imag();
    }
};

struct pos_dir_hash {
    inline std::size_t operator()(const std::tuple<Pos, Dir>& v) const {
        return std::get<0>(v).real()*859+std::get<0>(v).imag() * 149 + std::get<1>(v).real()*31 + std::get<1>(v).imag();
    }
};
inline std::pair<std::string, Strings> left(Strings&& strings)
{
	auto s = strings[0];
	strings.erase(strings.begin());
	return {s, strings};
}

inline std::pair<Strings, std::string> right(Strings&& strings)
{
	auto s = *strings.rbegin();
	strings.erase(strings.end() - 1);
	return {strings, s};
}

static constexpr const unsigned char OOB = 0xff;

static inline char peek(const Grid& grid, int i, int j)
{
    if(i < 0 || i >= grid.size() || j < 0 || j >= grid[i].size())
        return OOB;
    return grid[i][j];
}

static inline bool is_oob(const Grid& grid, Pos c)
{
	assert(!grid.empty() && !grid[0].empty());
	return c.imag() < 0 || c.imag() >= grid.size() || c.real() < 0 || c.real() >= grid[0].size();
}
static inline bool is_oob(const Grid& grid, int i, int j)
{
	return is_oob(grid, Pos{j, i});
}

static inline char& get(Grid& grid, Pos c)
{
	assert(!is_oob(grid, c));
	return grid[c.imag()][c.real()];
}

static inline char& get(Grid& grid, int i, int j)
{
	assert(!is_oob(grid, i, j));
	return grid[i][j];
}
static inline char peek(const Grid& grid, Pos c)
{
	if (is_oob(grid, c))
		return OOB;
	return grid[c.imag()][c.real()];
}

bool inline put(char what, int i, int j, Grid& grid)
{
    if(i < 0 || j < 0 || i >= grid.size() || j >= grid[0].size())
        return false;
    grid[i][j] = what;
    return true;
}

template<typename T>
std::vector<T> split(const std::string& line, const std::string& delim);

template<>
inline std::vector<std::string> split(const std::string& line, const std::string& delim)
{
    Strings ret;
    size_t start = 0;
    for(size_t j = line.find(delim, start); j != std::string::npos; start = j + delim.size(), j=line.find(delim, start))
    {
        ret.push_back(line.substr(start, j - start));
    }
    ret.push_back(line.substr(start));
    return ret;
}

template<>
inline std::vector<int> split(const std::string& line, const std::string& delim)
{
    Ints ret;
    size_t start = 0;
    for(size_t j = line.find(delim, start); j != std::string::npos; start = j + delim.size(), j=line.find(delim, start))
    {
        ret.push_back(std::stoi(line.substr(start, j - start)));
    }
    ret.push_back(std::stoi(line.substr(start)));
    return ret;
}

enum class InputType
{
	Test,
	Real
};
enum class Part
{
	One,
	Two
};

template<int _Day> struct DaySolver
{
    static const int Day = _Day;
	static inline std::string filename;

	static std::string part1();
	static std::string part2();

    static void solve(Part part, const std::string& input_dir, std::string expected_1st = "", std::string expected_2nd = "", bool time = false)
	{
		{
			char buf[32]{};
			snprintf(buf, sizeof(buf), "%s/day%d.txt", input_dir.c_str(), DaySolver::Day);
			filename=  buf;
		}
		auto start = std::chrono::steady_clock::now();
		std::string ans;
		std::string exp;
		if (part == Part::One)
		{
			ans = part1();
			exp = std::move(expected_1st);
		}
		else
		{
			ans = part2();
			exp = std::move(expected_2nd);
		}
		auto cnt = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - start).count();
		const char* quantity = "us";
		if (cnt > 1'000)
		{
			cnt /= 1'000;
			quantity = "ms";
		}
		if(cnt >= 1'000)
		{
			cnt /= 1'000;
			quantity = "s";
		}


		if (!exp.empty())
		{
			LOG("[%s] Expected \"%s\", got \"%s\"", exp == ans ? "OK" : "FAIL", exp.c_str(), ans.c_str());
		}
		else
			LOG("\"%s\"", ans.c_str());

		if (time)
			LOG("%s took %lld%s", part == Part::One ? "Part 1" : "Part 2", (long long)cnt, quantity);
	}
};
