# Builds ./SnakeArena (works with make on Linux, macOS and Windows/MinGW).
# You can also compile without make:  g++ -std=c++11 *.cpp -o SnakeArena
CXX = g++
CXXFLAGS = -g -std=c++11 -Wall
SRCS = $(wildcard *.cpp)

all: SnakeArena

SnakeArena: $(SRCS) $(wildcard *.h)
	$(CXX) $(CXXFLAGS) -o SnakeArena $(SRCS)

# Memory check (Linux only, needs valgrind):  make mem INPUT=inputs/t1_basic.txt
mem: all
	valgrind --leak-check=full --track-origins=yes ./SnakeArena $(INPUT)

.PHONY: all mem
