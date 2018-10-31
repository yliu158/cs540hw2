CPPFLAGS= -Wall -Wextra -pedantic

all: Map.hpp test-scaling.cpp
	g++ -std=c++11 test-scaling.cpp -o test

run:
	./test

checkmem:
	valgrind --leak-check=yes -v ./test

clean:
	-rm test
