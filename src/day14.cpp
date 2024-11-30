#include "common.h"

#include <array>
#include <regex>

static constexpr const int WIDTH = 101;
static constexpr const int HEIGHT = 103;

using Locations = std::unordered_map<int, Pos>;
using Velocities = std::unordered_map<int, Pos>;
std::pair<Locations, Velocities> parse(std::ifstream ifs)
{
	Locations locations;
	Velocities vels;
    int robot = 0;
    for(std::string line; std::getline(ifs, line); )
    {
        std::regex re1("p=(\\d+),(\\d+) v=(-?\\d+),(-?\\d+)");
        std::cmatch res;
        std::regex_match(line.c_str(), res, re1);
        if(res.size() == 5)
        {
            locations[robot] = Pos{std::stoi(res[1].str()), std::stoi(res[2].str())};
            vels[robot] = Pos{std::stoi(res[3].str()), std::stoi(res[4].str())};
            ++robot;
        }
    }
	return {locations, vels};
}

template<>
std::string DaySolver<14>::part1()
{
	auto [locations, vels] = parse(std::ifstream(filename));

	uint64_t res{};

    for(int iter = 0; iter < 100; ++iter)
    {
        for(auto& [r,loc] : locations)
        {
            loc += vels[r];
            if(loc.real() < 0) 
                loc = {loc.real() + WIDTH, loc.imag()};
            if(loc.imag() < 0) 
                loc = {loc.real(), loc.imag() + HEIGHT};
            loc = {loc.real() % WIDTH, loc.imag() % HEIGHT};
        }
	}

	std::array<int, 4> quads{};
	for(auto& [r,loc] : locations)
	{
		if(loc.real() < WIDTH/2 && loc.imag() < HEIGHT/2)
			++quads[2];
		else if(loc.real() < WIDTH/2 && loc.imag() > HEIGHT/2)
			++quads[3];
		else if(loc.real() > WIDTH/2 && loc.imag() < HEIGHT/2)
			++quads[1];
		else if(loc.real() > WIDTH/2 && loc.imag() > HEIGHT/2)
			++quads[0];
	}
	return std::to_string(quads[0] * quads[1] * quads[2] * quads[3]);
}

template<>
std::string DaySolver<14>::part2()
{
	auto [locations, vels] = parse(std::ifstream(filename));

	uint64_t res{};
	Grid grid(HEIGHT);
	for (auto &g : grid)
		g.resize(WIDTH, '.');
	bool found = false;
	//std::ofstream ofs("file.txt");
    for(int iter = 0; iter < 101*103 && !found; ++iter)
    {
#if 0
		double xs = 0;
		double ys = 0;
		std::array<int, 4> quads{};
		for(auto& [r,loc] : locations)
		{
			if(loc.real() < WIDTH/2 && loc.imag() < HEIGHT/2)
				++quads[2];
			else if(loc.real() < WIDTH/2 && loc.imag() > HEIGHT/2)
				++quads[3];
			else if(loc.real() > WIDTH/2 && loc.imag() < HEIGHT/2)
				++quads[1];
			else if(loc.real() > WIDTH/2 && loc.imag() > HEIGHT/2)
				++quads[0];
		}
		//ofs << quads[0]*quads[1]*quads[2]*quads[3] << '\x0a';
#endif

        for(auto& [r,loc] : locations)
        {
			grid[loc.imag()][loc.real()] = '.';
            loc += vels[r];
            if(loc.real() < 0) 
                loc = {loc.real() + WIDTH, loc.imag()};
            if(loc.imag() < 0) 
                loc = {loc.real(), loc.imag() + HEIGHT};
            loc = {loc.real() % WIDTH, loc.imag() % HEIGHT};
#if 0
			xs += loc.real();
			ys += loc.imag();
#endif
			grid[loc.imag()][loc.real()] = '#';
        }
#if 0
		xs /= locations.size();
		ys /= locations.size();
		double stddev_x{};
		double stddev_y{};
		std::unordered_map<int, int> freq;
#endif

		for (auto& [r, loc] : locations)
		{
			const int i = loc.imag();
			const int j = loc.real();
#if 0
			stddev_x += std::pow(loc.real() -xs, 2);
			stddev_y += std::pow(loc.imag() -ys, 2);
			++freq[j];
#endif
			if (
				   peek(grid, i - 1, j - 1) == '#' 
				&& peek(grid, i - 1, j + 1) == '#' 
				&& peek(grid, i - 2, j - 2) == '#' 
				&& peek(grid, i - 2, j + 2) == '#' 
				&& peek(grid, i - 1, j) == '#' 
				&& peek(grid, i - 1, j + 2) == '#'
			)
			{
				res = iter + 1;
				found = true;
				DLOG("FOUND! %d", i+1);
				for (auto &g : grid)
					DLOG("%s", g.c_str());
				break;
			}
		}
#if 0
		double entropy=0;
		for (auto& [r, loc] : locations)
		{
			auto pi =   double(freq[loc.real()])/locations.size();
			if(pi>0)
				entropy +=  pi*log2(pi);
		}
		entropy *= -1;
		stddev_x /= (locations.size());
		stddev_y /= (locations.size());
		//ofs << entropy << '\x0a';
		ofs << stddev_x << '\x0a';
#endif
    }
    return std::to_string(res);
}
