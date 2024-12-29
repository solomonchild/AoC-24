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
        if(ifs.eof())
        break;

        Ints nums;
        while (ifs.peek() != '\n' && !ifs.eof())
        {
            int a{};
            ifs >> a;
            nums.push_back(a);
        }

        uint64_t mask = uint64_t(std::pow(2, nums.size()-1) - 1);
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
        if(nums.empty())
            break;

        const auto SZ = nums.size();
        uint64_t mask = uint64_t(std::pow(2, SZ-1));
        bool done = false;
        std::array<uint64_t, 4096> pending_override{};
        size_t pending_len = 0;
        while (mask != UINT64_MAX)
        {
            pending_override[0] = uint64_t(nums[0]);
            pending_len = 1;

            for (int i = 0; i < SZ - 1; ++i)
            {
                const auto num = nums[i + 1];
                const size_t og_pending_len = pending_len;
                for(size_t j = 0; j < og_pending_len; ++j)
                {
                    pending_override[pending_len++] = pending_override[j] * uint64_t(std::pow(10, int(std::log10(num)) + 1)) + nums[i + 1];
                    assert(pending_len < pending_override.size());
                }
                const int op = ((mask >> (SZ - i - 2)) & 0x1);
                if (!op)
                {
                    for(size_t j = 0; j < og_pending_len; ++j)
                        pending_override[j] += num;
                }
                else
                {
                    for(size_t j = 0; j < og_pending_len; ++j)
                        pending_override[j] *= num;
                }
            }
            assert(pending_len < pending_override.size());
            for(size_t i = 0; i < pending_len; ++i)
            {
                if (pending_override[i] == res)
                {
                    final_res += res;
                    done = true;
                    break;
                }
            }
            if (done)
                break;
            --mask;
        }
    }
    return std::to_string(final_res);
}