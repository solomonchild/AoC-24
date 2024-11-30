#include "common.h"

struct my_pos_hash {
    inline std::size_t operator()(const std::pair<uint64_t, uint64_t>& v) const {
        return v.first*31+v.second;
    }
};
std::unordered_map<std::pair<uint64_t ,uint64_t>, uint64_t, my_pos_hash> memo;

uint64_t reduce(uint64_t num, int day) 
{
    auto stone = num;
    if(day == 0)
        return 1;
    if (memo.count({stone, day}))
        return memo[{stone, day}];
    uint64_t ret = 0;

    int len = int(std::log10(stone) + 1);
    if (!stone)
    {
        stone = 1;
    }
    else if (len % 2 == 0)
    {

        uint64_t pow = std::pow(10, len / 2);
        uint64_t left = stone / pow;
        uint64_t right = stone % pow;
        ret += reduce(right, day-1) + reduce(left, day-1);
        memo[{num, day}] = ret;
        return ret;
    }
    else
    {
        stone *= 2024;
    }
    return reduce(stone, day-1);
}

uint64_t day11(std::ifstream ifs, int days)
{
    uint64_t res{};
    std::vector<uint64_t> stones;
    while(ifs)
    {
        uint64_t num{};
        if(ifs.eof())
            break;
        ifs >> num;
        stones.push_back(num);
    }
    for(auto i : stones)
        res += reduce(i, days);
    return res;
}

template<>
std::string DaySolver<11>::part1()
{
    auto res = day11(std::ifstream(filename), 25);
    return std::to_string(res);
}

template<>
std::string DaySolver<11>::part2()
{
    auto res = day11(std::ifstream(filename), 75);
    return std::to_string(res);
}
