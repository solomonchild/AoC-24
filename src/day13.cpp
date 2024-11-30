#include "common.h"
#include <regex>

struct Instr
{
    int a_x = {};
    int a_y = {};

    int b_x = {};
    int b_y = {};
    uint64_t target_x = {};
    uint64_t target_y = {};
};

const int A_PUSH = 3;
const int B_PUSH = 1;
inline int gcd(int a, int b)
{
    while(b != 0)
        std::tie(a, b) = std::make_tuple(b, a%b);
    return a;
}

inline int lcm(int a, int b)
{
    if(a > b)
        return (a / gcd(a, b)) * b;
    else 
        return (b / gcd(a, b)) * a;
}

std::pair<uint64_t, uint64_t> get_price(const uint64_t target_x, const uint64_t target_y, Instr ins)
{
    int l = lcm(ins.b_x, ins.b_y);

    uint64_t x1 = l/ins.b_x*target_x;
    uint64_t x2 = l/ins.b_y*target_y;

    uint64_t a1 = l/ins.b_x*ins.a_x;
    uint64_t a2 = l/ins.b_y*ins.a_y;

    uint64_t b1 = l/ins.b_x*ins.b_x;
    uint64_t b2 = l/ins.b_y*ins.b_y;
    assert(b1 == b2);
    assert(a1 != a2);
    if(x1 < x2)
        std::swap(x1, x2);
    if(a1 < a2)
        std::swap(a1, a2);
    assert(x1 >= x2);
    assert(a1 >= a2);
    if(a1==a2 || ((x1-x2)%(a1-a2)) || (((target_x - (x1-x2)/(a1-a2) * ins.a_x)) % ins.b_x))
        return {0,0};
    uint64_t a_pushes = (x1-x2) / (a1-a2);
    if(a1==a2 || ((x1-x2)%(a1-a2)) || (((target_x - a_pushes * ins.a_x)) % ins.b_x))
        return {0,0};
    uint64_t b_pushes = (target_x - a_pushes * ins.a_x) / ins.b_x;
    if ((a_pushes * ins.a_x + b_pushes * ins.b_x != target_x) || (a_pushes * ins.a_y + b_pushes * ins.b_y != target_y))
    {
        return {0, 0};
    }
    return {a_pushes, b_pushes};
}

uint64_t solve_day13(std::string filename, bool part1 = true)
{
    uint64_t res{};
    std::ifstream ifs(filename);
    std::vector<Instr> instrs;
    std::regex re1("Button (.): X(\\+\\d*), Y(\\+\\d*)");
    std::regex re2("Prize: X=(\\d*), Y=(\\d*)");

    for(std::string line; std::getline(ifs, line);)
    {
        std::cmatch res;
        std::regex_match(line.c_str(), res, re1);
        if(res.size() == 4)
        {
            if(res[1].str() == "A")
            {
                instrs.push_back({});
                instrs.back().a_x = std::stoi(res[2].str());
                instrs.back().a_y = std::stoi(res[3].str());
            }
            else
            {
                instrs.back().b_x = std::stoi(res[2].str());
                instrs.back().b_y = std::stoi(res[3].str());
            }
        }
        else
        {
            std::regex_match(line.c_str(), res, re2);
            if(res.size() == 3)
            {
                instrs.back().target_x = std::stoi(res[1].str()) + 0;
                instrs.back().target_y = std::stoi(res[2].str()) + 0;
                if(!part1)
                {
                    instrs.back().target_x += 10000000000000;
                    instrs.back().target_y += 10000000000000;
                }
            }
        }
    }
    for(auto i : instrs)
    {
        auto p = get_price(i.target_x, i.target_y, i);
        assert(p.first >= 0 && p.second >=0);
        if(p.first || p.second)
            res += uint64_t(p.first*3 + p.second);
    }
    return res;
}

template<>
std::string DaySolver<13>::part1()
{
    return std::to_string(solve_day13(filename));
}


template<>
std::string DaySolver<13>::part2()
{
    return std::to_string(solve_day13(filename, false));
}
