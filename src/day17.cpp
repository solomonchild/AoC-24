#include "common.h"

#include <array>
#include <functional>

using Registers = std::array<uint64_t, 3>;
struct Context
{
	Registers regs;
	int ip;
	std::vector<uint64_t> out;
};
using Op = std::function<void(Context&, int)>;

uint64_t decode_combo(Registers& regs, int v)
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
			return [](Context& c, int operand)
			{
				int val = int(std::pow(2, decode_combo(c.regs, operand)));
				c.regs[0] = c.regs[0]/val;
				c.ip += 2;
				DLOG("adv %d", operand);
			};
			break;
		}
		case 1:
		{
			return [](Context& c, int operand)
			{
				c.regs[1] = c.regs[1]^operand;
				c.ip += 2;
				DLOG("bxl %d", operand);
			};
			break;
		}
		case 2:
		{
			return [](Context& c, int operand)
			{
				c.regs[1] = decode_combo(c.regs, operand)%8;
				c.ip += 2;
				DLOG("bst %d", operand);
			};
			break;
		}
		case 3:
		{
			return [](Context& c, int operand)
			{
				DLOG("jnz %d", operand);
				if(!c.regs[0])
				{
					c.ip +=2;
					return;
				}
				c.ip = operand;
			};
			break;
		}
		case 4:
		{
			return [](Context& c, int operand)
			{
				DLOG("bxc");
				c.regs[1] = c.regs[1]^c.regs[2];
				c.ip += 2;
			};
			break;
		}
		case 5:
		{
			return [](Context& c, int operand)
			{
				DLOG("out %d", operand);
				int dec = decode_combo(c.regs, operand)%8;
				c.out.push_back(dec);
				c.ip += 2;
			};
			break;
		}
		case 6:
		{
			return [](Context& c, int operand)
			{
				DLOG("bdv %d", operand);
				int val = int(std::pow(2, decode_combo(c.regs, operand)));
				c.regs[1] = c.regs[0]/val;
				c.ip += 2;
			};
			break;
		}
		case 7:
		{
			return [](Context& c, int operand)
			{
				DLOG("cdv %d", operand);
				int val = int(std::pow(2, decode_combo(c.regs, operand)));
				if(val)
					c.regs[2] = c.regs[0]/val;
				c.ip += 2;
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

	Context c{};
	c.regs=regs;
	
	while(c.ip != program.size())
	{
		auto fn = decode_opcode(program[c.ip]);
		fn(c, program[c.ip+1]);
	}
	assert((c.out == std::vector<uint64_t>{1,0,2,0,5,7,2,1,3}));

	std::string res;
	for(auto o : c.out)
		res += std::to_string(o) + ",";
	res.erase(res.end()-1);
	return res;
}
std::pair<uint64_t, uint64_t> step(uint64_t a, const std::vector<int>& program) 
{
	Context c{};
	c.regs[0] = a;
	
	while(c.out.empty() && c.ip < program.size())
	{
		auto fn = decode_opcode(program[c.ip]);
		fn(c, program[c.ip+1]);
	}
	
	return {uint64_t(c.regs[0]), c.out.empty()?UINT64_MAX:uint64_t(c.out[0])};
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
		//const uint64_t b = (((_a & 0x7) ^ 7) ^ 7 ^ (_a >> ((_a & 0x7) ^ 7)))&0x7;
		const auto [_, b] = step(_a, program);
		assert(b >= 0);
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
	uint64_t a{64012472};
	std::vector<int> out;
	while(a) {
		uint64_t cur{};
		std::tie(a, cur) = step(a, program);
		out.push_back(cur);
	} 
	assert((out == std::vector<int>{ 1,0,2,0,5,7,2,1,3 }));
	assert((simulate(64012472, program) == std::vector<int>{ 1,0,2,0,5,7,2,1,3 }));
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
