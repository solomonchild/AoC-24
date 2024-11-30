#include "common.h"
#include <stack>
#include <sstream>

std::tuple<Grid, Pos, std::string> parse_grid(std::ifstream ifs)
{
	Pos start{};
    int row = 0;
    Grid grid;
    for (std::string line; std::getline(ifs, line);)
    {
		if(line.empty())
			break;
        grid.push_back({});
        grid.back().reserve(line.size());
        for(int i = 0; i < line.size(); ++i)
        {
            grid.back().push_back(line[i]);
            if(line[i] == '@')
                start = {i, row};
        }
        ++row;
    }
	std::string moves;
	for(std::string line; std::getline(ifs, line);)
	{
		moves += line;
	}
	DLOG("%s", moves.c_str());
    return std::tuple<Grid, Pos, std::string>{grid, start, moves};
}

Pos do_move(Pos pos, Pos dir, Grid& grid)
{
	assert(peek(grid, pos) == '@');
	if(peek(grid, pos + dir) == '#')
	{
		return pos;
	}
	else if(peek(grid, pos + dir) == '.')
	{
		get(grid, pos) = '.';
		get(grid, pos+dir) = '@';
		return pos+dir;
	}
	else
	{
		int boxes = 0;
		while(peek(grid, pos + dir * (boxes+1)) == 'O')
		{
			++boxes;
		}
		
		if(peek(grid, pos + dir * (boxes+1)) == '#')
		{
			return pos;
		}
		assert(peek(grid, pos + dir * (boxes+1)) == '.');
		while(boxes)
		{
			get(grid, pos+dir*(boxes+1)) = 'O';
			--boxes;
		}
		get(grid, pos) = '.';
		get(grid, pos+dir) = '@';
		return pos+dir;
	}
}
static std::unordered_map<char, Pos> CHAR_TO_DIR
{
	{'v', {0, 1}},
	{'>', {1, 0}},
	{'<', {-1, 0}},
	{'^', {0, -1}},
};

template<>
std::string DaySolver<15>::part1()
{
	auto [grid, start, moves] = parse_grid(std::ifstream(filename));
	Pos pos = start;
	for(char move : moves)
	{
		pos = do_move(pos, CHAR_TO_DIR[move], grid);
		print(grid);
	}
	uint64_t res{};

	for(auto i = 0; i < grid.size(); ++i)
	{
		for(auto j = 0; j < grid[0].size(); ++j)
		{
			if(peek(grid, i, j) == 'O')
			{
				res += i*100 + j;
			}
		}
	}

	return std::to_string(res);
}

std::tuple<Grid, Pos, std::string> parse_grid2(std::ifstream& ifs)
{
	Pos start{};
    Grid grid;
    for (std::string line; std::getline(ifs, line);)
    {
		if(line.empty())
			break;
        grid.push_back({});
        grid.back().reserve(line.size()*2);
        for(int i = 0; i < line.size(); ++i)
        {
			if(line[i] == '.')
			{
				grid.back().push_back(line[i]);
				grid.back().push_back(line[i]);
			}
			else if(line[i] == '#')
			{
				grid.back().push_back(line[i]);
				grid.back().push_back(line[i]);
			}
			else if(line[i] == '@')
			{
				grid.back().push_back(line[i]);
				grid.back().push_back('.');
			}
			else if(line[i] == 'O')
			{
				grid.back().push_back('[');
				grid.back().push_back(']');
			}
        }
    }
	for(int i = 0; i < grid.size(); ++i)
	{
		for(int j = 0; j < grid[0].size(); ++j)
		{
			if(peek(grid, i, j) == '@')
			{
				start = {j, i};
				break;
			}
		}
	}
	std::string moves;
	for(std::string line; std::getline(ifs, line);)
	{
		moves += line;
	}
	print(grid);
    return std::tuple<Grid, Pos, std::string>{grid, start, moves};
}

Pos do_move2(Pos pos, Pos dir, Grid& grid)
{
	assert(peek(grid, pos) == '@');
	if(peek(grid, pos + dir) == '#')
		return pos;
	else if(peek(grid, pos + dir) == '.')
	{
		get(grid, pos) = '.';
		get(grid, pos+dir) = '@';
		return pos+dir;
	}
	else
	{
		if(dir == Pos{1, 0} || dir == Pos{-1, 0})
		{
			int boxes = 1;
			while(peek(grid, pos + dir * boxes) == '[' || peek(grid, pos + dir * boxes) == ']')
			{
				++boxes;
			}

			if(peek(grid, pos + dir * boxes) == '#')
			{
				return pos;
			}

			//invert, since coming from the ack
			bool open = peek(grid, pos + dir) != '[';
			while(boxes)
			{
				get(grid, pos+dir*boxes) = open ? '[' : ']';
				open = !open;
				--boxes;
			}
			get(grid, pos) = '.';
			pos+=dir;
			get(grid, pos) = '@';
		}
		else
		// vertical
		{
			int og_start = (pos+dir).real();
			int og_end = og_start + 1;
			if(peek(grid, (pos+dir).imag(), og_start) == ']')
			{
				--og_start;
				--og_end;
			}
			int start = og_start;
			int end = og_end;
			int boxes = 1;
			// count
			std::stack<std::vector<int>> lvl_boxes;
			lvl_boxes.push({});
			lvl_boxes.top().push_back({og_start});
			int prev_start = 0;
			int prev_end = 0;
			while(true)
			{
				const int y = (pos+dir*boxes).imag();
				const int x = (pos+dir*boxes).real();

				if(peek(grid, y, start) == ']') 
					--start;
				if(peek(grid, y, end) == '[') 
					++end;

				{
					int dots = 0;
					for(int i = start; i <= end; ++i)
					{
						if(get(grid,y,i) == '.')
							++dots;
						else if(get(grid,y,i) == '#')
							return pos;
					}

					if(dots == end-start+1)
						break;
				}

				--start;
				++end;
				for(; grid[y][start] != '[' && start <= end; ++start);
				for(; grid[y][end] != ']' && start <= end; --end);

				prev_start = start;
				prev_end = end;
				lvl_boxes.push({});
				for(int i = start; i <= end; ++i)
				{
					if(grid[y][i] == '[')
					{
						lvl_boxes.top().push_back({i});
					}
				}
				++boxes;
			}

			while(!lvl_boxes.empty())
			{
				int lvl = lvl_boxes.size();
				const int y = (pos+dir*lvl).imag();
				const int prev_y = (pos+dir*(lvl-1)).imag();
				const int x = (pos+dir*lvl).real();

				auto boxes = lvl_boxes.top(); lvl_boxes.pop();
				for(auto b : boxes)
				{
					if(grid[y][b] == '#' || grid[y][b+1] == '#')
						break;
					grid[y][b] = '[';
					grid[y][b+1] = ']';

					if(!lvl_boxes.empty())
					{
						grid[prev_y][b] = '.';
						grid[prev_y][b+1] = '.';
					}
				}
			}
			get(grid, pos) = '.';
			pos += dir;
			grid[pos.imag()][og_start] = '.';
			grid[pos.imag()][og_end] = '.';
			get(grid, pos) = '@';
		}
		return pos;
	}
}

uint64_t calc(Grid& grid)
{
	uint64_t res{};

	for(auto i = 0; i < grid.size(); ++i)
	{
		for(auto j = 0; j < grid[0].size(); ++j)
		{
			if(peek(grid, i, j) == '[')
			{
				res += i*100 + j;
			}
		}
	}
	return res;
}

static void test()
{
	auto from_str= [](std::string str){
		std::istringstream iss(str);
		Grid grid;
		Pos pos;
		for(std::string line; std::getline(iss, line); )
		{
			if(line.empty())
				continue;
			grid.push_back(line);
			if(int i = line.find("@"); i != std::string::npos)
			{
				pos = {i, int(grid.size()-1)};
			}
		}
		return std::make_pair(grid, pos);
	};

	auto run = [from_str](std::string case1, std::string expected, std::string moves){
		auto [grid, pos] = from_str(case1);
		DLOG("------Test case------");
		print(grid);
		for(auto move : moves)
		{
			DLOG("Move %c", move);
			pos = do_move2(pos, CHAR_TO_DIR[move], grid);
			print(grid);
		}
		print(grid);
		auto [exp_grid, exp_pos] = from_str(expected);
		print(exp_grid);
		DLOG("%d,%d vs %d,%d", pos.real(), pos.imag(), exp_pos.real(), exp_pos.imag());
		DLOG("Calc: %lld", calc(grid));
		assert(pos == exp_pos);
		assert(grid == exp_grid);
	};
	{
		std::string case1=R"<!(
#########
##.....##
##.....##
##..[].##
##..@..##
#########
)<!";

		std::string expected=R"<!(
#########
##.....##
##..[].##
##..@..##
##.....##
#########
)<!";
		run(case1, expected, "^");
	}
	{
		std::string case1=R"<!(
#########
##.....##
##..@..##
##..[].##
##.....##
#########
)<!";

		std::string expected=R"<!(
#########
##.....##
##.....##
##..@..##
##..[].##
#########
)<!";
		run(case1, expected, "v");
	}

	{
		std::string case1=R"<!(
#############
##.........##
##..@......##
##..[].....##
##.[][]....##
##.........##
##.........##
#############
)<!";

		std::string expected=R"<!(
#############
##.........##
##.........##
##..@......##
##..[].....##
##.[][]....##
##.........##
#############
)<!";
		run(case1, expected, "v");
	}
	{
		std::string case1=R"<!(
#############
##.........##
##.........##
##..[].....##
##.[][]....##
##..@......##
##.........##
#############
)<!";

		std::string expected=R"<!(
#############
##.........##
##..[].....##
##.[]......##
##..@[]....##
##.........##
##.........##
#############
)<!";
		run(case1, expected, "^");
	}

	{
		std::string case1=R"<!(
#############
##.........##
##..@......##
##..[].....##
##.[].#....##
##[]#......##
##.........##
#############
)<!";

		std::string expected=R"<!(
#############
##.........##
##..@......##
##..[].....##
##.[].#....##
##[]#......##
##.........##
#############
)<!";
		run(case1, expected, "v");
	}
	{
		std::string case1=R"<!(
#############
##.........##
##..@......##
##..[].....##
##.[].#....##
##[].......##
##.........##
#############
)<!";

		std::string expected=R"<!(
#############
##.........##
##.........##
##..@......##
##..[]#....##
##.[]......##
##[].......##
#############
)<!";
		run(case1, expected, "v");
	}
	{
		std::string case1=R"<!(
#############
##.........##
##.........##
##..[].....##
##.[].#....##
##[].......##
##.@.......##
#############
)<!";

		std::string expected=R"<!(
#############
##.........##
##..[].....##
##.[]......##
##[]..#....##
##.@.......##
##.........##
#############
)<!";
		run(case1, expected, "^");
	}
	{
		std::string case1=R"<!(
#############
##.........##
##.........##
##.........##
##.@[]#....##
##.........##
##.........##
#############
)<!";

		std::string expected=R"<!(
#############
##.........##
##.........##
##.........##
##.@[]#....##
##.........##
##.........##
#############
)<!";
		run(case1, expected, ">");
	}
	{
		std::string case1=R"<!(
#############
##.........##
##.........##
##....#....##
##.@[][][].##
##....#....##
##.........##
#############
)<!";

		std::string expected=R"<!(
#############
##.........##
##.........##
##....#....##
##..@[][][]##
##....#....##
##.........##
#############
)<!";
		run(case1, expected, ">");
	}

	{
		std::string case1=R"<!(
#############
##.........##
##.........##
##....#....##
##..@[][][]##
##....#....##
##.........##
#############
)<!";

		std::string expected=R"<!(
#############
##.........##
##.........##
##....#....##
##..@[][][]##
##....#....##
##.........##
#############
)<!";
		run(case1, expected, ">");
	}
	{
		std::string case1=R"<!(
#############
##..[].....##
##...@.....##
#############
)<!";

		std::string expected=R"<!(
#############
##..[].....##
##...@.....##
#############
)<!";
		run(case1, expected, "^");
	}

	{
		std::string case1=R"<!(
##############
##......##..##
##..........##
##....[][]@.##
##....[]....##
##..........##
##############
)<!";

		std::string expected=R"<!(
##############
##...[].##..##
##...@.[]...##
##....[]....##
##..........##
##..........##
##############
)<!";
		run(case1, expected, "<vv<<^^<<^^");
	}

	{
		std::string case1=R"<!(
####################
##....[]....[]..[]##
##............[]..##
##..[][]....[]..[]##
##....[]@.....[]..##
##[]##....[]......##
##[]....[]....[]..##
##..[][]..[]..[][]##
##........[]......##
####################
)<!";

		std::string expected=R"<!(
####################
##[].......[].[][]##
##[]...........[].##
##[]........[][][]##
##[]......[]....[]##
##..##......[]....##
##..[]............##
##..@......[].[][]##
##......[][]..[]..##
####################
)<!";

		run(case1, expected, "<vv>^<v^>v>^vv^v>v<>v^v<v<^vv<<<^><<><>>v<vvv<>^v^>^<<<><<v<<<v^vv^v>^vvv<<^>^v^^><<>>><>^<<><^vv^^<>vvv<>><^^v>^>vv<>v<<<<v<^v>^<^^>>>^<v<v><>vv>v^v^<>><>>>><^^>vv>v<^^^>>v^v^<^^>v^^>v^<^v>v<>>v^v^<v>v^^<^^vv<<<v<^>>^^^^>>>v^<>vvv^><v<<<>^^^vv^<vvv>^>v<^^^^v<>^>vvvv><>>v^<<^^^^^^><^><>>><>^^<<^^v>>><^<v>^<vv>>v>>>^v><>^v><<<<v>>v<v<v>vvv>^<><<>^><^>><>^v<><^vvv<^^<><v<<<<<><^v<<<><<<^^<v<^^^><^>>^<v^><<<^>>^v<v^v<v^>^>>^v>vv>^<<^v<>><<><<v<<v><>v<^vv<<<>^^v^>^^>>><<^v>>v^v><^^>>^<>vv^<><^^>^^^<><vvvvv^v<v<<>^v<v>v<<^><<><<><<<^^<<<^<<>><<><^^^>^^<>^>v<>^^>vv<^v^v<vv>^<><v<^v>^^^>>>^^vvv^>vvv<>>>^<^>>>>>^<<^v>^vvv<>^<><<v>v^^>>><<^^<>>^v^<v^vv<>v^<<>^<^v^v><^<<<><<^<v><v<>vv>>v><v^<vv<>v^<<^");
	}

	//calc
	{
		std::string fin=R"<!(
####################
##[].......[].[][]##
##[]...........[].##
##[]........[][][]##
##[]......[]....[]##
##..##......[]....##
##..[]............##
##..@......[].[][]##
##......[][]..[]..##
####################
)<!";
		auto [grid, _] = from_str(fin);
		assert(calc(grid) == 9021);
	}
}


template<>
std::string DaySolver<15>::part2()
{
	test();
#if 0
	exit(1);
#endif
	auto [grid, start, moves] = parse_grid2(std::ifstream(filename));
	Pos pos = start;
	for(auto move : moves)
	{
		auto prev_grid = grid;
		pos = do_move2(pos, CHAR_TO_DIR[move], grid);
		for(auto i = 0; i < grid.size(); ++i)
		{
			for(auto j = 0; j < grid[0].size(); ++j)
			{
				if(peek(grid, i, j) == '[' && peek(grid, i, j+1) != ']')
					assert(false);
				if(peek(grid, i, j) == ']' && peek(grid, i, j-1) != '[')
					assert(false);
			}
		}
	}

	return std::to_string(calc(grid));
}
