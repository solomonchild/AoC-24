#include "common.h"

#include <fstream>
#include <algorithm>
#include <string>
#include <vector>

template<>
std::string DaySolver<7>::part1()
{
    std::ifstream ifs(filename);
    long long final_res = 0;
    while(ifs)
    {
        long long res{};
        ifs >> res;
        ifs.ignore();

        Ints nums;
        while (ifs.peek() != '\n' && !ifs.eof())
        {
            int a{};
            ifs >> a;
            nums.push_back(a);
        }

        uint64_t mask = std::pow(2, nums.size()-1) - 1;
        while(mask != UINT64_MAX)
        {

            long long pending = nums[0];
            for(int i = 0; i < nums.size()-1; ++i)
            {
                int op = ((mask >> (nums.size()-i-2)) & 0x1);
                if (!op)
                    pending = pending + nums[i+1];
                else 
                    pending = pending * nums[i+1];
                if(pending > res)
                    break;
            }
            if(pending == res)
            {
                final_res += res;
                break;
            }
            --mask;
        }
    }
    return std::to_string(final_res);
}

template<>
std::string DaySolver<7>::part2()
{
    // Should've gone for recursion on this one, which would result in 3^N calls
    // This approach loops a bit less than (2^(N-1)-1)^2 * N. There's culling, 
    // but it doesn't do effective work, so the algorithm still overshoots
    std::ifstream ifs(filename);
    uint64_t final_res = 0;
    int line= 0;
    while(ifs)
    {
        uint64_t res{};
        ifs >> res;
        ifs.ignore();

        Ints nums;
        while (ifs.peek() != '\n' && !ifs.eof())
        {
            int a{};
            ifs >> a;
            nums.push_back(a);
        }

        auto SZ = nums.size();
        uint64_t second = std::pow(2, SZ - 1) - 1;
        const uint64_t MAX = std::pow(2, SZ-1);
        while(second != UINT64_MAX)
        {
            uint64_t mask = MAX;
            auto max = MAX;
            bool done = false;
            while (mask != UINT64_MAX)
            {
                while(max != 0 && ((mask&max) == 0))
                    max >>= 1;
                if(max & second)
                {
                    while(max & second)
                        max >>= 1;
                    if(!max)
                        mask = 0;
                    else
                        mask = (max << 1) - 1;
                }

                uint64_t pending = nums[0];
                for (int i = 0; i < SZ - 1; ++i)
                {
                    const auto num = nums[i+1];
                    int op_override = ((second >> (SZ - i - 2)) & 0x1);
                    int op = ((mask >> (SZ - i - 2)) & 0x1);
                    if (op_override)
                        pending = pending * std::pow(10, int(std::log10(num))+1) + nums[i + 1];
                    else if (!op)
                        pending = pending + num;
                    else
                        pending = pending * num;
                    if (pending > res)
                        break;
                }
                if (pending == res)
                {
                    final_res += res;
                    done = true;
                    break;
                }
                --mask;
            }
            if (done)
                break;
            --second;
        }
    }
    return std::to_string(final_res);
}