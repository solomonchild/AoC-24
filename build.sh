mkdir -p bin
ARGS=""
FILES=`ls src/*cpp`

for var in "$@"
do
	if [ "$var" = "--unity" ]; then
		ARGS="$ARGS -DUNITY"
		FILES="src/main.cpp"
	fi
	if [ "$var" = "--prof" ]; then
		ARGS="$ARGS -g3"
	fi
done

time clang++ $ARGS -O3 -Iinclude --std=c++17 $FILES -o bin/aoc