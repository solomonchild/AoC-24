#include "common.h"

#include <fstream>
#include <string>
#include <vector>

using Map = std::unordered_map<char, std::vector<std::pair<int, int>>>;

std::pair<Grid, Map> parse_map(std::ifstream ifs)
{
    Grid grid;
    Map map;
    int row = 0;
    for(std::string line; std::getline(ifs, line);)
    {
        for(int j = 0; j < line.size(); ++j)
        {
            if(std::isalnum(line[j]))
            {
                map[line[j]].push_back({row, j});
            }
        }
        grid.push_back(line);
        ++row;
    }
    return {grid, map};
}

template<>
std::string DaySolver<8>::part1()
{
    auto [grid, map] = parse_map(std::ifstream(filename));
    for(auto freq_pos : map)
    {
        for(int i = 0; i < freq_pos.second.size(); ++i)
        {
            auto _t = freq_pos.second[i];
            for(int j = i+1; j < freq_pos.second.size(); ++j)
            {
                auto pos_a = freq_pos.second[i];
                auto pos_b = freq_pos.second[j];
                const int dx = pos_a.first - pos_b.first;
                const int dy = pos_a.second - pos_b.second;

                int x1, y1;
                std::tie(x1, y1) = pos_a;
                int x2, y2;
                std::tie(x2, y2) = pos_b;

                put('#', x1+dx, y1+dy, grid);
                put('#', x2-dx, y2-dy, grid);
            }
        }
    }
    long res = 0;
    for(auto g : grid)
    {
        for(auto i : g)
            if(i == '#' || i == '?')
                ++res;

    }
    return std::to_string(res);
}

template<>
std::string DaySolver<8>::part2()
{
    auto [grid, map] = parse_map(std::ifstream(filename));
    for(auto freq_pos : map)
    {
        for(int i = 0; i < freq_pos.second.size(); ++i)
        {
            auto _t = freq_pos.second[i];
            for(int j = i+1; j < freq_pos.second.size(); ++j)
            {
                auto pos_a = freq_pos.second[i];
                auto pos_b = freq_pos.second[j];
                const int dx = (pos_a.first - pos_b.first);
                const int dy = (pos_a.second - pos_b.second);
                int x = pos_a.first;
                int y = pos_a.second;

                int i = 0;
                while (put('#', x + i * dx, y + i * dy, grid))
                {
                    ++i;
                }

                i = 0;
                while (put('#', x - i * dx, y - i * dy, grid))
                {
                    ++i;
                }
            }
        }
    }
    long res = 0;
    for(auto g : grid)
    {
        for(auto i : g)
            if(i == '#')
                ++res;
    }
    return std::to_string(res);
}
