#include "common.h"

#include <cassert>
#include <complex>
#include <fstream>
#include <string>
#include <unordered_set>
#include <vector>

using Pos = std::complex<int>;
using Dir = std::complex<int>;

static const Pos RTURN = Pos{0, 1};
static const Pos START_DIR = Pos{0, -1};

std::pair<Grid, Pos> parse(std::ifstream ifs)
{
    int row = 0;

    Grid map;
    Pos pos;
    for(std::string line; std::getline(ifs, line); )
    {
        map.push_back(line);
        if(auto f = line.find("^"); f != std::string::npos)
        {
            pos = {int(f), row};
        }
        ++row;
    }
    return {map, pos};
}

template<>
std::string DaySolver<6>::part1()
{
    auto [map, pos] = parse(std::ifstream(filename));
    Pos dir = START_DIR;
    std::unordered_set<Pos, pos_hash> visited;

    while(true)
    {
        visited.insert(pos);

        auto t = pos+dir;

        if (is_oob(map, t))
            break;
        else if (peek(map, t) == '#')
            dir *= RTURN;
        else
        {
            get(map, pos) = '.';
            pos = t;
            get(map, t) = '@';
        }
    }
    return std::to_string(visited.size());
}

template<>
std::string DaySolver<6>::part2()
{
    auto [map, pos] = parse(std::ifstream(filename));
    auto get_coords = [pos=pos, map=map] (Pos start_pos) mutable
    {
        Pos dir = START_DIR;
        std::unordered_set<Pos, pos_hash> visited;

        while(true)
        {
            if (pos != start_pos)
                visited.insert(pos);
            auto t = pos+dir;

            if (is_oob(map, t))
                break;
            else if (peek(map, t) == '#')
                dir *= RTURN;
            else
            {
                pos = t;
            }
        }
        return visited;
    };

    auto run = [](Pos pos, const Grid &map, std::size_t size_hint) -> bool
    {
        std::unordered_set<std::tuple<Pos, Dir>, pos_dir_hash> visited;
        visited.reserve(size_hint);
        Pos dir = START_DIR;
        while(true)
        {
            auto t = pos+dir;

            if (is_oob(map, t))
            {
                return false;
            }
            else if (peek(map, t) == '#')
            {
                dir *= std::complex<int> {0, 1};
                if (!visited.insert({pos, dir}).second)
                {
                    return true;
                }
            }
            else
            {
                pos = t;
            }
        }
        assert(false);
    };

    auto coords = get_coords(pos);

    long res = 0;
    for(auto& p : coords)
    {
        get(map, p) = '#';
        if (run(pos, map, coords.size()))
            ++res;
        get(map, p) = '.';

    }
    return std::to_string(res);
}
