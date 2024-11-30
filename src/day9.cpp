#include "common.h"

#include <fstream>
#include <algorithm>
#include <string>
#include <vector>

template<>
std::string DaySolver<9>::part1()
{
    std::ifstream ifs(filename);
    std::string line;
    std::getline(ifs, line);

    std::vector<long long> vec;
    vec.reserve(line.size() * 5);

    uint64_t id = 0;
    for(int i = 0; i < line.size(); ++i)
    {
        const int f = line[i] - '0';
        if(!f)
            continue;
        if(i%2 == 0)
        {
            for(int j = 0; j < f; ++j)
            {
                vec.push_back(id);
            }
            ++id;
        }
        else
        {
            int idx = vec.size();
            for(int j = 0; j < f; ++j)
            {
                vec.push_back(-1);
            }
        }
    }
    int move_idx = vec.size()-1;
    int write_idx = 0;
    while(write_idx < move_idx)
    {
        while(vec[move_idx] == -1)
            --move_idx;
        while(vec[write_idx] != -1)
            ++write_idx;
        if(write_idx >= move_idx)
            break;
        vec[write_idx] = vec[move_idx];
        vec[move_idx] = -1;
    }
    uint64_t res{};
    for(int i =0 ;i < vec.size(); ++i)
    {
        if (vec[i] == -1)
            continue;
        res += i * (vec[i]);
    }
    return std::to_string(res);
}

template<>
std::string DaySolver<9>::part2()
{
    std::ifstream ifs(filename);
    std::string line;
    std::getline(ifs, line);

    std::vector<uint64_t> vec;
    vec.reserve(line.size() * 5);
    std::vector<std::pair<int, int>> free_list;
    const int PT = -1;

    {
        uint64_t id = 0;
        for (int i = 0; i < line.size(); ++i)
        {
            const int f = (line[i] - '0');
            if (!f)
                continue;
            if (i % 2 == 0)
            {
                for (int j = 0; j < f; ++j)
                {
                    vec.push_back(id);
                }
                ++id;
            }
            else
            {
                if (!free_list.empty() && free_list.back().first + free_list.back().second == vec.size())
                {
                    free_list.back().second += f;
                }
                else
                    free_list.push_back({int(vec.size()), f});

                for (int j = 0; j < f; ++j)
                {
                    vec.push_back(PT);
                }
            }
        }
    }
    std::sort(free_list.begin(), free_list.end(), [](const auto& a, const auto& b){
        return a.first < b.first;
    });
    long long move_idx = vec.size()-1;
    while(move_idx >= 0)
    {
        while(move_idx >= 0 && vec[move_idx] == PT)
            --move_idx;
        if(move_idx <= 0)
            break;
        const int start = move_idx;
        const int id = vec[move_idx];
        while(move_idx >= 0 && vec[move_idx] == id)
            --move_idx;
            
        if(move_idx <= 0)
            break;
        ++move_idx;
        const int span = start - move_idx + 1;
        assert(span > 0);
        for(int fi = 0; fi < free_list.size(); ++fi)
        {
            auto& p = free_list[fi];
            if(!p.second)
                continue;

            assert(p.first != move_idx);
            if(p.first > move_idx)
                break;

            if(p.second >= span && p.first < move_idx)
            {
                for (int i = p.first; i < p.first + span; ++i)
                {
                    assert(vec[i] == PT);
                    vec[i] = id;
                }
                for (int i = move_idx; i <= start; ++i)
                {
                    assert(vec[i] == id);
                    vec[i] = PT;
                }
                assert(span <= p.second);
                p.second -= span;
                p.first += span;
                break;
            }
        }
        --move_idx;
    }
    uint64_t res{};
    for(int i =0 ;i < vec.size(); ++i)
    {
        if (vec[i] == PT)
            continue;
        res += i * vec[i];
    }
    return std::to_string(res);
}
