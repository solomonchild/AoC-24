#include "common.h"

std::tuple<int, int, int> count_region(const Grid& grid, Pos pos, std::unordered_set<Pos, pos_hash>& seen)
{
    const auto rn = peek(grid, pos);
    if (rn == OOB)
        return {};
    if (!seen.insert(pos).second)
        return {};

    int area = 1;
    int angles = 0;
    int perimeter = 0;
    for (auto diag : NEIGHBOR_DIAG_DELTAS)
    {
        const auto h = Pos{0, diag.imag()};
        const auto v = Pos{ diag.real(), 0 };
        const bool hh = peek(grid, pos + h) == rn;
        const bool vv = peek(grid, pos + v) == rn;

        if ((peek(grid, pos + diag) != rn) && !(vv ^ hh) || (!hh && !vv))
        {
            ++angles;
        }
    }

    for (auto i : NEIGHBOR_DELTAS)
    {
        if (peek(grid, pos + i) != rn)
        {
            ++perimeter;
            continue;
        }

        auto ans = count_region(grid, pos + i, seen);
        area += std::get<0>(ans);
        angles += std::get<1>(ans);
        perimeter += std::get<2>(ans);
    }
    return {area, angles, perimeter};
}

template<int SecondIndex>
static uint64_t solve_day12(const std::string& filename)
{
    std::ifstream ifs(filename);
    Grid grid;
    for(std::string line; std::getline(ifs, line);)
    {
        grid.push_back(line);
    }

    uint64_t res{};
    std::unordered_set<Pos, pos_hash> seen;
    for(auto i = 0; i < grid.size(); ++i)
    {
        for(auto j = 0; j < grid.size(); ++j)
        {
            auto t = count_region(grid, {i, j}, seen);
            res += std::get<0>(t) * std::get<SecondIndex>(t);
        }
    }
    return res;
}

template<>
std::string DaySolver<12>::part1()
{
    return std::to_string(solve_day12<2>(filename));
}


template<>
std::string DaySolver<12>::part2()
{
    return std::to_string(solve_day12<1>(filename));
}
