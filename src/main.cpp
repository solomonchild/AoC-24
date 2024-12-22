#include <stdio.h>
#include <string.h>

#include <array>
#include <string>
#include <functional>
#include <charconv>

#include "common.h"

#ifdef UNITY
// #include "day1.cpp"
// #include "day2.cpp"
// #include "day3.cpp"
// #include "day4.cpp"
// #include "day5.cpp"
// #include "day6.cpp"
// #include "day7.cpp"
// #include "day8.cpp"
// #include "day9.cpp"
// #include "day10.cpp"
// #include "day11.cpp"
// #include "day12.cpp"
// #include "day13.cpp"
// #include "day14.cpp"
// #include "day15.cpp"
// #include "day16.cpp"
// #include "day17.cpp"
// #include "day18.cpp"
// #include "day19.cpp"
// #include "day20.cpp"
//#include "day21.cpp"
// #include "day22.cpp"
#include "day23.cpp"
#endif

using FuncAnsT = std::tuple<std::function<void(Part, InputType, std::string, std::string, bool)>, std::string, std::string>;
std::array Solutions= {
	// FuncAnsT{&DaySolver<1>::solve, "1258579", "23981443"},
	// FuncAnsT{&DaySolver<2>::solve, "369", "428"},
	// FuncAnsT{&DaySolver<3>::solve, "187194524", "127092535"},
	// FuncAnsT{&DaySolver<4>::solve, "2524", "1873"},
	// FuncAnsT{&DaySolver<5>::solve, "5747", "5502"},
	// FuncAnsT{&DaySolver<6>::solve, "4967", "1789"},
	// FuncAnsT{&DaySolver<7>::solve, "5837374519342", "492383931650959"},
	// FuncAnsT{&DaySolver<8>::solve, "303", "1045"},
	// FuncAnsT{&DaySolver<9>::solve, "6283404590840", "6304576012713"},
	// FuncAnsT{&DaySolver<10>::solve, "782", "1694"},
	// FuncAnsT{&DaySolver<11>::solve, "203228", "240884656550923"},
	// FuncAnsT{&DaySolver<12>::solve, "1465968", "897702"},
	// FuncAnsT{&DaySolver<13>::solve, "39996", "73267584326867"},
	// FuncAnsT{&DaySolver<14>::solve, "225648864", "7847"},
	// FuncAnsT{&DaySolver<15>::solve, "1451928", "1462788"},
	// FuncAnsT{&DaySolver<16>::solve, "66404", "433"},
	// FuncAnsT{&DaySolver<17>::solve, "1,0,2,0,5,7,2,1,3", "265652340990875"},
	// FuncAnsT{&DaySolver<18>::solve, "292", "58,44"},
	// FuncAnsT{&DaySolver<19>::solve, "338", "841533074412361"},
	// FuncAnsT{&DaySolver<20>::solve, "1448", "1017615"},
	// FuncAnsT{&DaySolver<21>::solve, "156714", "191139369248202"},
	// FuncAnsT{&DaySolver<22>::solve, "18694566361", "2100"},
	FuncAnsT{&DaySolver<23>::solve, "", ""},
	// FuncAnsT{&DaySolver<24>::solve, "", ""},
	// FuncAnsT{&DaySolver<25>::solve, "", ""},
};

bool DEBUG = false;
bool TIME = false;

int main(int argc, char** argv)
{
	int day = -1;
	int part = -1;
	char type = 'r';
	for(int i = 1; i < argc; ++i)
	{
		std::string arg = argv[i];
		if (arg == "--time" || arg == "-t") 
		{
			TIME = true;
		} 
		else if (arg == "--debug") 
		{
			DEBUG = true;
		} 
		else if(arg == "--input" || arg == "-i")
		{
			if (i + 1 >= argc || ('r' != argv[i+1][0] && 't' != argv[i+1][0]))
			{
				LOG("Expected either 'r' or 't' for --type");
				exit(1);
			}
			else
			{
				type = argv[i+1][0];
				++i;
			}
		}
		else if(arg == "--day" || arg == "-d")
		{
			if (i + 1 >= argc)
			{
				LOG("Expected [1; 31] after --day/-d");
				exit(1);
			}
			else
			{
				int val{};
				auto res = std::from_chars((const char*)argv[i+1], (const char*) argv[i+1] + strlen(argv[i+1]), val);
				if (res.ec != std::errc{} || val < 1 || val > 31)
				{
						LOG("Expected [1; 31] after --day/-d, got \"%s\"", argv[i+1]);
						exit(1);
				}
				else
				{
					day = val;
					++i;
				}
			}
		}
		else if(arg == "--part" || arg == "-p")
		{
			if (i + 1 >= argc)
			{
				LOG("Expected [1; 2] after --part/-p");
				exit(1);
			}
			else
			{
				int val{};
				auto res = std::from_chars((const char*)argv[i+1], (const char*) argv[i+1] + strlen(argv[i+1]), val);
				if (res.ec != std::errc{} || val < 1 || val > 31)
				{
						LOG("Expected [1; 2] after --part/-p, got \"%s\"", argv[i+1]);
						exit(1);
				}
				else
				{
					part = val;
					++i;
				}
			}
		}
		else
		{
			LOG("Unkown argument \"%s\"", arg.c_str());
			exit(1);
		}
	}

	auto solve_day = [](int day, int part, char type)
	{
		auto [solve_func, ans1, ans2] = Solutions[day-1];
		LOG("\nSolving day %d", day);
		const auto input_type = type == 'r' ? InputType::Real : InputType::Test;

		if (part != -1)
			solve_func(Part(part-1), input_type, ans1, ans2, TIME);
		else
		{
			solve_func(Part::One, input_type, ans1, ans2, TIME);
			solve_func(Part::Two, input_type, ans1, ans2, TIME);
		}
	};

	if (day != -1)
	{
		solve_day(day, part, type);
	}
	else
	{
		for(auto i = 0; i < Solutions.size(); ++i)
			solve_day(i+1, part, type);
	}
}
