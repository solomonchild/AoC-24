#include "common.h"

#include <array>
#include <functional>

using Registers = std::array<int, 3>;
using Op = std::function<void(Registers&, int operand, int& ip, std::vector<int>& output)>;

int decode_combo(Registers& regs, int v)
{
	assert(v != 7);
	if(v >= 0 && v <= 3)
		return v;
	if(v >= 4 && v<=6)
		return regs[v-4];
	assert(false);
	return -1;
}

Op decode_opcode(int v)
{
	switch(v)
	{
		case 0:
		{
			return [](Registers& regs, int operand, int& ip, std::vector<int>&)
			{
				int val = int(std::pow(2, decode_combo(regs, operand)));
				regs[0] = regs[0]/val;
				ip += 2;
				DLOG("adv %d", operand);
			};
			break;
		}
		case 1:
		{
			return [](Registers& regs, int operand, int& ip, std::vector<int>&)
			{
				regs[1] = regs[1]^operand;
				ip += 2;
				DLOG("bxl %d", operand);
			};
			break;
		}
		case 2:
		{
			return [](Registers& regs, int operand, int& ip, std::vector<int>&)
			{
				regs[1] = decode_combo(regs, operand)%8;
				ip += 2;
				DLOG("bst %d", operand);
			};
			break;
		}
		case 3:
		{
			return [](Registers& regs, int operand, int& ip, std::vector<int>&)
			{
				DLOG("jnz %d", operand);
				if(!regs[0])
				{
					ip +=2;
					return;
				}
				ip = operand;
			};
			break;
		}
		case 4:
		{
			return [](Registers& regs, int, int& ip, std::vector<int>&)
			{
				DLOG("bxc");
				regs[1] = regs[1]^regs[2];
				ip += 2;
			};
			break;
		}
		case 5:
		{
			return [](Registers& regs, int operand, int& ip, std::vector<int>& out)
			{
				DLOG("out %d", operand);
				int dec = decode_combo(regs, operand)%8;
				out.push_back(dec);
				ip += 2;
			};
			break;
		}
		case 6:
		{
			return [](Registers& regs, int operand, int& ip, std::vector<int>&)
			{
				DLOG("bdv %d", operand);
				int val = int(std::pow(2, decode_combo(regs, operand)));
				regs[1] = regs[0]/val;
				ip += 2;
			};
			break;
		}
		case 7:
		{
			return [](Registers& regs, int operand, int& ip, std::vector<int>&)
			{
				DLOG("cdv %d", operand);
				int val = int(std::pow(2, decode_combo(regs, operand)));
				regs[2] = regs[0]/val;
				ip += 2;
			};
			break;
		}
		default:
		{
			assert(false);
			return {};
		}
	}

}

std::pair<Registers, std::vector<int>> parse(std::ifstream ifs)
{
	Registers regs;
	std::vector<int> program;
	for(std::string line; std::getline(ifs, line);)
	{
		if(line.find("Register A:") != std::string::npos)
		{
			int i = std::stoi(split<std::string>(line, "Register A: ")[1]);
			regs[0]=i;
		}
		else if(line.find("Register B:") != std::string::npos)
		{
			int i = std::stoi(split<std::string>(line, "Register B: ")[1]);
			regs[1]=i;
		}
		else if(line.find("Register C:") != std::string::npos)
		{
			int i = std::stoi(split<std::string>(line, "Register C: ")[1]);
			regs[2]=i;
		}
		else if(line.find("Program: ") != std::string::npos)
		{
			std::string pg = split<std::string>(line, "Program: ")[1];
			for(auto v : split<std::string>(pg, ","))
				program.push_back(std::stoi(v));
		}
	}
	return {regs, program};
}

template<>
std::string DaySolver<17>::part1()
{
	auto [regs, program] = parse(std::ifstream(filename));

	std::vector<int> out;
	int ip = 0;
	
	while(ip != program.size())
	{
		auto fn = decode_opcode(program[ip]);
		fn(regs, program[ip+1], ip, out);
	}
	assert((out == std::vector<int>{1,0,2,0,5,7,2,1,3}));

	std::string res;
	for(auto o : out)
		res += std::to_string(o) + ",";
	res.erase(res.end()-1);
	return res;
}

auto _find (uint64_t res, const std::vector<int>& program, uint64_t a, int idx)
{
	if(idx ==-1)
	{
		return res;
	}
	const uint64_t expected = program[idx];
	for(int i = 0; i <= 7; ++i)
	{ 
		auto _a = a + i;
		const uint64_t b = (((_a & 0x7) ^ 7) ^ 7 ^ (_a >> ((_a & 0x7) ^ 7)))&0x7;
		if (expected == b)
		{
			if (auto r = _find((res<<3)+i, program, _a<<3, idx - 1); r != UINT64_MAX)
				return r;
		}
	}
	return UINT64_MAX;
};

template<>
std::string DaySolver<17>::part2()
{
	auto [regs, program] = parse(std::ifstream(filename));
	auto simulate = [](uint64_t aval, const std::vector<int>& program) 
	{
		uint64_t a = aval;
		uint64_t b = 0;
		uint64_t c = 0;

		std::vector<int> out;
		while(a != 0)
		{
			b = (((a & 0x7) ^ 7) ^ 7 ^ (a >> ((a & 0x7) ^ 7)));
			a>>=3;

			out.push_back(b&0x7);
		}
		return out;
	};
#if 0
	for(int i = 1; i < 1000000; i*=8)
	{
		DLOG("%d", i);
		auto out = simulate(i-1, program);
		DLOG("For %d out len is %zu", i-1, out.size()); 
		out = simulate(i, program);
		DLOG("For %d out len is %zu", i, out.size()); 
	}
#endif

	// test that the RE attempt was successfull
	assert((simulate(64012472, program) == std::vector<int>{ 1,0,2,0,5,7,2,1,3 }));
	auto res = _find(0, program, 0, program.size()-1);
	return std::to_string(res);
}
