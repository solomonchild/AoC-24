#include "common.h"
#include <queue>
#include <stack>

std::vector<Pos> get_n(const Grid& grid, Pos cur)
{
    std::vector<Pos> ret;
    if (is_oob(grid, cur))
        return ret;
    const char chr = peek(grid, cur);
    for(auto i : {Pos{1, 0}, Pos{0, 1}, Pos{-1,0}, Pos{0,-1}})
    {
        const auto c = cur + i;
        if(is_oob(grid, c))
            continue;
        const auto c_chr = peek(grid, c);
        if(chr + 1 != c_chr)
            continue;
        if(c_chr != '.')
            ret.push_back(c);
    }
    return ret;
}

std::pair<Grid, std::vector<Pos>> parse_grid(std::ifstream ifs)
{
    std::vector<Pos> starts{};
    int row = 0;
    Grid grid;
    for (std::string line; std::getline(ifs, line);)
    {
        grid.push_back({});
        grid.back().reserve(line.size());
        for(int i = 0; i < line.size(); ++i)
        {
            grid.back().push_back(line[i]);
            if(line[i] == '0')
                starts.push_back({i, row});
        }
        ++row;
    }
    return {grid, starts};
}

uint64_t count_trails(const std::vector<Pos>& starts, const Grid& grid, bool distinct_paths)
{
    uint64_t res = 0;
    for (auto i = 0; i < starts.size(); ++i)
    {
        auto start = starts[i];
        std::unordered_set<Pos, pos_hash> seen;
        std::stack<Pos> stack;
        stack.push(start);
        while (!stack.empty())
        {
            auto cur = stack.top(); stack.pop();
            if (!distinct_paths)
            {
                if(!seen.insert(cur).second)
                    continue;
            }
            const char cur_char = peek(grid, cur);
            if (cur_char == '9')
                ++res;
            auto ns = get_n(grid, cur);
            for(const auto& n : ns)
            {
                if (cur_char + 1 == peek(grid, n))
                    stack.push(n);
            }
        }
    }
    return res;
}

template<>
std::string DaySolver<10>::part1()
{
    auto [grid, starts] = parse_grid(std::ifstream(filename));
    return std::to_string(count_trails(starts, grid, false));
}

template<>
std::string DaySolver<10>::part2()
{
    auto [grid, starts] = parse_grid(std::ifstream(filename));
    return std::to_string(count_trails(starts, grid, true));
}
