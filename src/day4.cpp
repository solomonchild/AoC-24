#include "common.h"

#include <fstream>
#include <string>
#include <vector>

static Grid get_grid(std::ifstream ifs)
{
    Grid grid;
    {
        std::string line;
        while (std::getline(ifs, line))
        {
            grid.push_back(line);
        }
    }
    return grid;
}

static inline bool check (const Grid& grid, int i, int j, int dir_x, int dir_y)
{
    return peek(grid, i + dir_x, j + dir_y) == 'M' &&
            peek(grid, i + dir_x * 2, j + dir_y * 2) == 'A' &&
            peek(grid, i + dir_x * 3, j + dir_y * 3) == 'S';
}

template<>
std::string DaySolver<4>::part1()
{
    auto grid = get_grid(std::ifstream(filename));

    long long res{};
    REP(i, grid.size())
    {
        REP(j, grid[i].size())
        {
            if (peek(grid, i, j) == 'X')
            {
                for (int x : {-1, 0, 1})
                    for (int y : {-1, 0, 1})
                        if (check(grid, i, j, x, y))
                            ++res;
            }
        }
    }
    return std::to_string(res);
}

template<>
std::string DaySolver<4>::part2()
{
    auto grid = get_grid(std::ifstream(filename));
    long long res{};

    REP(i, grid.size())
    {
        REP(j, grid[i].size())
        {
            if(peek(grid, i, j) == 'A')
            {
                if (
                   (check(grid, i + 2, j - 2, -1, +1) || check(grid, i - 2, j + 2, +1, -1)) 
                && (check(grid, i - 2, j - 2, +1, +1) || check(grid, i + 2, j + 2, -1, -1))
                )
                ++res;

            }
        }
    }
    return std::to_string(res);
}