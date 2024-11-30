#include "common.h"

#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>


using AdjMat = std::unordered_map<int, std::unordered_set<int>>;
using Degrees = std::unordered_map<int, int>;

inline bool is_cr(std::ifstream& ifs)
{
    return ifs.peek() == '\r';
}
inline bool is_newline(std::ifstream& ifs)
{
    return ifs.peek() == '\n';
}

inline void ignore_newline(std::ifstream& ifs)
{
    while (is_cr(ifs))
        ifs.ignore();
    if (is_newline(ifs))
        ifs.ignore();
    while (is_cr(ifs))
        ifs.ignore();
}

static std::pair<AdjMat, Degrees>  parse(std::ifstream& ifs)
{
    AdjMat adj;
    Degrees deg;
    while(ifs)
    {
        ignore_newline(ifs);
        if (is_newline(ifs))
        {
            ifs.ignore();
            break;
        }
        int a{}, b{};
        ifs >> a; ifs.ignore(); ifs >> b;
        adj[a].insert(b);
        ++deg[b];
    }
    return {adj, deg};
}

inline long long check_pt1(AdjMat& adj, const std::vector<int>& nums)
{
    bool correct = true;
    if (nums.empty())
        return 0;

    for (int i = 0; i < nums.size() - 1; ++i)
    {
        if (!adj[nums[i]].count(nums[i+1]))
        {
            correct = false;
            break;
        }
    }
    return correct ? nums[nums.size()/2] : 0;
};

template<>
std::string DaySolver<5>::part1()
{
    std::ifstream ifs(filename);
    auto [adj, _] = parse(ifs);

    std::vector<int> nums;
    long long res{};
    while(ifs)
    {
        if(is_newline(ifs) || is_cr(ifs))
        {
            res += check_pt1(adj, nums);
            nums.clear();
        }

        int a{};
        ifs >> a;
        nums.push_back(a);

        if(ifs.peek() == ',')
            ifs.ignore();
    }
    res += check_pt1(adj, nums);
    return std::to_string(res);
}

inline long long check_pt2(AdjMat& adj, std::vector<int> &nums)
{
    std::unordered_map<int, int> _inc;
    std::unordered_set<int> seen (nums.begin(), nums.end());
    if (nums.empty())
        return 0;
    for (auto i : nums)
    {
        for (auto out : adj[i])
            if (seen.count(out))
                ++_inc[out];
    }
    for (int i = 0; i < nums.size() - 1; ++i)
    {
        if (!adj[nums[i]].count(nums[i + 1]))
        {
            std::vector<int> sorted;
            sorted.reserve(nums.size());
            std::queue<int> nodes;
            for (auto i : nums)
            {
                if (!_inc[i])
                    nodes.push(i);
            }

            while (!nodes.empty())
            {
                auto n = nodes.front();
                nodes.pop();
                sorted.push_back(n);

                for (auto m : adj[n])
                {
                    --_inc[m];
                    if (_inc[m] == 0 && seen.count(m))
                    {
                        nodes.push(m);
                    }
                }
            }
            return sorted[sorted.size() / 2];
        }
    }
    return 0;
}

template<>
std::string DaySolver<5>::part2()
{
    std::ifstream ifs(filename);
    auto [adj, inc] = parse(ifs);

    std::vector<int> nums;
    long long res{};

    while(ifs)
    {
        if(is_newline(ifs) || is_cr(ifs))
        {
            res += check_pt2(adj, nums);
            nums.clear();
        }
        int a{};
        ifs >> a;
        nums.push_back(a);

        if(ifs.peek() == ',')
            ifs.ignore();
    }
    res += check_pt2(adj, nums);
    return std::to_string(res);
}
