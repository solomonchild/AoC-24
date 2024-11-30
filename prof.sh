rm callgrind*
valgrind --tool=callgrind bin/aoc $@
