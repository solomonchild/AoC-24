#include "common.h"

using Map = umap<std::string, uset<std::string>>;
size_t max = 0;
void find_max_cliques(uset<std::string>&& R, uset<std::string>&& P, uset<std::string>&& X, Map& graph, std::vector<uset<std::string>>& max_cliques)
{
	if(P.empty() && X.empty() && R.size() > max)
	{
		max_cliques.push_back(R);
		max = std::max(R.size(), max);
		return;
	}
	while(!P.empty())
	{
		auto v = *P.begin();
		auto new_R = R;
		new_R.insert(v);
		uset<std::string> new_P;
		uset<std::string> new_X;
		for(auto N : graph[v])
		{
			if(P.count(N))
				new_P.insert(N);
			if(X.count(N))
				new_X.insert(N);
		}

		find_max_cliques(std::move(new_R), std::move(new_P), std::move(new_X), graph, max_cliques);
		P.erase(v);
		X.insert(v);
	}
}

template <>
std::string DaySolver<23>::part1()
{
	Map map;
	std::ifstream ifs(filename);
	uset<std::string> names;
	for(std::string line; std::getline(ifs, line);)
	{
		auto nodes = split<std::string>(line, "-");
		map[nodes[0]].insert(nodes[1]);
		map[nodes[1]].insert(nodes[0]);
		names.insert(nodes[0]);
		names.insert(nodes[1]);
	}

	int res=0;
	uset<std::string> triplets;
	for(const auto& [c1, ns] : map)
	{
		for(const auto& c2 : ns)
		{
			for(const auto& c3 : map[c2])
			{
				if(c1[0] != 't' && c2[0] != 't' && c3[0] != 't')
					continue;
				if(map[c3].count(c1))
				{
					auto cc1 = c1;
					auto cc2 = c2;
					auto cc3 = c3;
					if (cc1 > cc3)
						std::swap(cc1, cc3);
					if (cc2 > cc3)
						std::swap(cc2, cc3);
					if (cc1 > cc2)
						std::swap(cc1, cc2);
					//triplets.insert(names[cc1]*10000 + names[cc2]*100 + names[cc3]);
					triplets.insert(cc1+cc2+cc3);
				}
			}
		}
	}
	return std::to_string(triplets.size());
}


template <>
std::string DaySolver<23>::part2()
{
	Map map;
	std::ifstream ifs(filename);
	uset<std::string> names;
	for (std::string line; std::getline(ifs, line);)
	{
		auto nodes = split<std::string>(line, "-");
		map[nodes[0]].insert(nodes[1]);
		map[nodes[1]].insert(nodes[0]);
		names.insert(nodes[0]);
		names.insert(nodes[1]);
	}
	std::vector<uset<std::string>> cliques;
	find_max_cliques({}, std::move(names), {}, map, cliques);
	int res = 0;
	uset<std::string> max_c;
	size_t max = 0;

	for (auto &c : cliques)
	{
		if (c.size() > max)
		{
			max = c.size();
			max_c = c;
		}
	}
	std::string pwd;
	Strings ss{max_c.begin(), max_c.end()};
	sort(ss.begin(), ss.end());
	for (auto cc : ss)
		pwd += cc + ",";
	pwd.pop_back();
	return pwd;
}