#include "common.h"

#include <set>
struct Gate
{
	std::string wire1;
	std::string gate;
	std::string wire2;
	std::string out;
	std::strong_ordering operator<=>(const Gate&) const = default;

	std::string to_str() const
	{
		char buf[32+1]{};
		snprintf(buf, 32, "{%s %s %s -> %s}", wire1.c_str(), gate.c_str(), wire2.c_str(), out.c_str());
		return buf;
	}
};

using Gates = umap<std::string, Gate>;
using InGates = umap<std::string, std::vector<Gate>>;

int get_val(Gates& gates, umap<std::string, int>& values, const std::string& out)
{
	if(gates.count(out))
	{
		auto op = gates[out].gate;
		int a = get_val(gates, values, gates[out].wire1);
		int b = get_val(gates, values, gates[out].wire2);
		int c=0;
		if(op=="AND")
		{
			c = a&&b;
		}
		else if(op == "OR")
		{
			c = a||b;
		}
		else if(op == "XOR")
		{
			c = a^b;
		}
		return c;
	}
	return values[out];
}

template <>
std::string DaySolver<24>::part1()
{
	std::ifstream ifs(filename);

	bool vals = false;
	std::vector<std::string> to_find;

	umap<std::string, Gate> gates;
	umap<std::string, int> values;
	for(std::string line; std::getline(ifs, line);)
	{
		if(line.empty())
		{
			vals = true;
			continue;
		}
		if(!vals)
		{

			auto s = split<std::string>(line, ":");
			for(auto& ss : s)
			{
				int i = 0;
				for(; i < ss.size(); ++i)
				{
					if(ss[i] != ' ')
						break;
				}
				if(i != 0)
					ss = ss.substr(i);
			}
			values[s[0]] = std::stoi(s[1]);
		}
		else
		{
			auto parts = split<std::string>(line, " ");
			Gate g{
				parts[0],
				parts[1],
				parts[2],
				parts[4],

			};
			gates[parts[4]] = g;
			if (parts[4][0] == 'z')
			{
				to_find.push_back(parts[4]);
			}
		}
	}
	std::sort(to_find.begin(), to_find.end());
	uint64_t res = 0;
	for(auto i = 0; i < to_find.size(); ++i)
	{
		auto needed_out = to_find[i];
		uint64_t v = get_val(gates, values, needed_out);
		res = res | (v<<i);
	}
	return std::to_string(res);
}

enum ErrorType
{
	expected_and_got_z,
	expected_and_x00,
	expected_or_after_and,
	expected_xor,
	expected_xor_and,
	expected_xor_and_size_ne2,
	expected_z
};

std::string to_str(ErrorType et)
{
	switch(et)
	{
	case expected_xor:
		return "XOR expected";
	case expected_xor_and:
		return "XOR and AND expected";
	case expected_xor_and_size_ne2:
		return "XOR or AND expected, but size is not 2";
	case expected_z:
		return "Z expected";
	case expected_and_x00:
		return "AND expected (x00)";
	case expected_or_after_and:
		return "Expected OR after AND";
	case expected_and_got_z:
		return "Got Z, expected AND";
	default:
		assert(false);
		return "";
	}
}

struct Error
{
	ErrorType type;
	Gate gate1;
	Gate gate2;
	std::strong_ordering operator<=>(const Error&) const = default;
	std::string to_str() const
	{
		auto es = ::to_str(type);
		if(!gate1.gate.empty())
			es += "(" + gate1.to_str();
		if(!gate2.gate.empty())
			es += ", " + gate2.to_str();
		es += ")";
		return es;
	}
};
using Errors = std::set<Error>;

std::string last_z;
auto check_z (Errors& errors, const Gate& z_gate)
{
	if(z_gate.out == last_z)
		return;
	if(z_gate.gate != "XOR")
	{
		errors.insert({ErrorType::expected_xor, z_gate});
	}
}
void check_after_xor (Errors& errors, const Gate& gate, InGates& in_gates, int lvl, bool x00 = false);
void check_after_and(Errors& errors, const Gate& gate, InGates& in_gates, bool x00 = false);
void check_after_or(Errors& errors, const Gate& gate, InGates& in_gates) 
{
	if(gate.out[0] == 'z')
	{
		check_z(errors, gate);
		//return;
	}
	auto gates = in_gates[gate.out];

	if(gate.out[0] == 'z')
	{
		check_z(errors, gate);
	}
	else if(gates.size() != 2)
	{
		errors.insert({ErrorType::expected_xor_and_size_ne2, gates[0]});
		// return;
	}
	else if(gates[0].gate == "XOR" && gates[1].gate == "AND")
	{
		check_after_xor(errors, gates[0], in_gates,2);
		check_after_and(errors, gates[1], in_gates);
	}
	else if(gates[0].gate == "AND" && gates[1].gate == "XOR")
	{
		check_after_xor(errors, gates[1], in_gates,2);
		check_after_and(errors, gates[0], in_gates);
	}
}

void check_after_and(Errors& errors, const Gate& gate, InGates& in_gates, bool x00) 
{
	if(gate.out[0] == 'z')
	{
		errors.insert({ErrorType::expected_and_got_z, gate});
		return;
	}
	auto gates = in_gates[gate.out];
	if(gate.out[0] == 'z')
	{
		check_z(errors, gate);
	}
	else if(gates.size() != 1 && !x00)
	{
		errors.insert({ErrorType::expected_or_after_and, gate});
		// return;
	}
	else if(x00)
	{
		if(gates[0].gate != "AND")
		{
			errors.insert({ErrorType::expected_and_x00, gates[0]});
		}
	}
	else
		check_after_or(errors, gates[0], in_gates);
}

void check_after_xor (Errors& errors, const Gate& gate, InGates& in_gates, int lvl, bool x00) 
{
	if (lvl == 2)
	{
		if (gate.out[0] == 'z')
		{
			check_z(errors, gate);
			//return;
		}
		else
		{
			errors.insert({ErrorType::expected_z, gate});
			 return;
		}
	}
	const auto& gates = in_gates[gate.out];
	if(gate.out[0] == 'z')
	{
		check_z(errors, gate);
	}
	else
	{
		if (gates.size() != 2)
		{
			errors.insert({ErrorType::expected_xor_and_size_ne2, gate});
		}
		else if (gates[0].gate != "XOR" && gates[0].gate != "AND")
		{
			errors.insert({ErrorType::expected_xor_and, gates[0], gates[1]});
		}
		else if (gates[0].gate == "XOR")
		{
			check_after_xor(errors, gates[0], in_gates, lvl + 1);
			check_after_and(errors, gates[1], in_gates);
		}
		else if (gates[0].gate == "AND")
		{
			check_after_xor(errors, gates[1], in_gates, lvl + 1);
			check_after_and(errors, gates[0], in_gates);
		}
	}
}

template <>
std::string DaySolver<24>::part2()
{
	std::ifstream ifs(filename);

	bool vals = false;
	std::vector<std::string> zs;
	std::vector<std::string> xs;
	std::vector<std::string> ys;

	Gates gates;
	InGates in_gates;
	umap<std::string, int> values;

	for(std::string line; std::getline(ifs, line);)
	{
		if(line.empty())
		{
			vals = true;
			continue;
		}
		if(!vals)
		{
			auto s = split<std::string>(line, ":");
			for(auto& ss : s)
			{
				int i = 0;
				for(; i < ss.size(); ++i)
				{
					if(ss[i] != ' ')
						break;
				}
				if(i != 0)
					ss = ss.substr(i);
			}
			values[s[0]] = std::stoi(s[1]);
			if (s[0][0] == 'y')
			{
				ys.push_back(s[0]);
			}
			else if (s[0][0] == 'x')
			{
				xs.push_back(s[0]);
			}
		}
		else
		{
			auto parts = split<std::string>(line, " ");
			Gate g{
				parts[0],
				parts[1],
				parts[2],
				parts[4],

			};
			gates[parts[4]] = g;
			if (parts[4][0] == 'z')
			{
				zs.push_back(parts[4]);
			}
			in_gates[parts[0]].push_back(g);
			in_gates[parts[2]].push_back(g);
		}
	}

	std::sort(xs.begin(), xs.end());
	std::sort(ys.begin(), ys.end());
	std::sort(zs.begin(), zs.end());
	last_z = zs.back();

	assert(xs.size() == ys.size());

	Errors errors;
	for(int i = 0; i < zs.size(); ++i)
	{
		//last z will have OR
		if (i != zs.size() - 1)
		;
			//check_z(errors, gates, zs[i]);
	}

	for(int i = 0; i < xs.size(); ++i)
	{
		auto x = xs[i];
		auto y = ys[i];
		auto cur_gates = in_gates[x];
		assert(cur_gates.size() == 2);
		for(auto& gate : cur_gates)
			assert((gate.wire1 == x && gate.wire2 == y) || (gate.wire1 == y && gate.wire2 == x));

		int lvl = 1;
		bool x00 = false;
		if(x == "x00")
		{
			++lvl;
			x00 = true;
		}
		if(cur_gates[0].gate == "XOR" && cur_gates[1].gate == "AND")
		{
			check_after_xor(errors, cur_gates[0], in_gates, lvl, x00);
			check_after_and(errors, cur_gates[1], in_gates, x00);
		}
		else if(cur_gates[0].gate == "AND" && cur_gates[1].gate == "XOR")
		{
			check_after_xor(errors, cur_gates[1], in_gates, lvl, x00);
			check_after_and(errors, cur_gates[0], in_gates, x00);
		}
		else
		{
			errors.insert({ErrorType::expected_xor_and, cur_gates[0], cur_gates[1]});
		}
	}
	// assert(errors.size() == 8);
	// assert(errors.count(Error{ErrorType::expected_xor, Gate{"kgd", "OR", "kqf", "z10"}}));
	// assert(errors.count(Error{ErrorType::expected_z, Gate{"sqr", "XOR", "mwq", "mwk"}}));
	// assert(errors.count(Error{ErrorType::expected_z, Gate{"nfh", "XOR", "nqq", "qgd"}}));
	// assert(errors.count(Error{ErrorType::expected_or_after_and, Gate{"y24", "AND", "x24", "jmh"}}));
	// assert(errors.count(Error{ErrorType::expected_xor_and_size_ne2, Gate{"y24", "XOR", "x24", "hsw"}}));
	// assert(errors.count(Error{ErrorType::expected_z, Gate{"wwp", "XOR", "cvt", "gqp"}}));
	// assert(errors.count(Error{ErrorType::expected_and_got_z, Gate{"cvt", "AND", "wwp", "z33"}}));
	// assert(errors.count(Error{ErrorType::expected_and_got_z, Gate{"y18", "AND", "x18", "z18"}}));

	Strings outs;
	for(auto& e : errors)
	{
		outs.push_back(e.gate1.out);
	}
	std::sort(outs.begin(), outs.end());
	std::string ans;
	for(auto& o : outs)
		ans += o + ",";
	ans.pop_back();

	return ans;
}
