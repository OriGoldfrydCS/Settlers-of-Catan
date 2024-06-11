# Email: origoldbsc@gmail.com

# (1) To compile the code, run 'make'.
# (2) To run the game, execute './Catan' in the terminal.
# (3) To run a simulation of one round, execute './main' after building the test target with 'make main'.
# (4) To run tests, execute './test' after building the test target with 'make test'.

# Compiler settings
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Werror -Wsign-conversion -g

# Valgrind settings
VALGRIND_FLAGS = -v --leak-check=full --show-leak-kinds=all --error-exitcode=99

# Source files and headers
SOURCES = board.cpp player.cpp tile.cpp catan.cpp resources.cpp intersection.cpp edge.cpp vertex.cpp cards.cpp
HEADERS = board.hpp player.hpp tile.hpp catan.hpp resources.hpp intersection.hpp edge.hpp vertex.hpp cards.hpp  

# Object files
OBJS = board.o player.o tile.o catan.o resources.o intersection.o edge.o vertex.o cards.o

# Test sources
TEST_SRC = test.cpp test_counter.cpp
TEST_OBJS = test.o test_counter.o

# Executable names
GAME_EXEC = Catan
TEST_EXEC = test
MAIN_EXEC = main

# Default build target
all: $(GAME_EXEC)

# Game executable
$(GAME_EXEC): $(OBJS) catanmain.o
	$(CXX) $(CXXFLAGS) -o Catan board.o player.o tile.o catan.o resources.o intersection.o edge.o vertex.o cards.o catanmain.o

# Main executable
$(MAIN_EXEC): $(OBJS) main.o
	$(CXX) $(CXXFLAGS) -o main board.o player.o tile.o catan.o resources.o intersection.o edge.o vertex.o cards.o main.o

# Test executable
$(TEST_EXEC): $(TEST_OBJS) player.o board.o tile.o resources.o intersection.o edge.o vertex.o cards.o catan.o
	$(CXX) $(CXXFLAGS) -o test $(TEST_OBJS) player.o board.o tile.o resources.o intersection.o edge.o vertex.o cards.o catan.o
	./test

# Object compilation
board.o: board.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c -o board.o board.cpp

player.o: player.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c -o player.o player.cpp

tile.o: tile.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c -o tile.o tile.cpp

catan.o: catan.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c -o catan.o catan.cpp

resources.o: resources.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c -o resources.o resources.cpp

intersection.o: intersection.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c -o intersection.o intersection.cpp

edge.o: edge.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c -o edge.o edge.cpp

vertex.o: vertex.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c -o vertex.o vertex.cpp

cards.o: cards.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c -o cards.o cards.cpp

test.o: test.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c -o test.o test.cpp

test_counter.o: test_counter.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c -o test_counter.o test_counter.cpp

# Run Valgrind
valgrind: $(GAME_EXEC) $(TEST_EXEC)
	valgrind --tool=memcheck $(VALGRIND_FLAGS) ./Catan 2>&1 | { egrep "lost| at " || true; }
	valgrind --tool=memcheck $(VALGRIND_FLAGS) ./test 2>&1 | { egrep "lost| at " || true; }

# Clean up command to remove all compiled files
clean:
	rm -f *.o main Catan test
