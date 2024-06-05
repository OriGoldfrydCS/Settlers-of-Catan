# Compiler settings
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Werror -Wsign-conversion -g

# Valgrind settings
VALGRIND_FLAGS = -v --leak-check=full --show-leak-kinds=all --error-exitcode=99

# Source files and headers
SOURCES = board.cpp player.cpp tile.cpp catan.cpp resources.cpp intersection.cpp edge.cpp vertex.cpp cards.cpp 
HEADERS = board.hpp player.hpp tile.hpp catan.hpp resources.hpp intersection.hpp edge.hpp vertex.hpp cards.hpp  

# Object files
OBJS = $(SOURCES:.cpp=.o)

# Test sources
TEST_SRC = test.cpp test_counter.cpp
TEST_OBJS = $(TEST_SRC:.cpp=.o)

# Executable names
GAME_EXEC = Catan
TEST_EXEC = tests

# Default build target
all: $(GAME_EXEC)

# Game executable
$(GAME_EXEC): $(OBJS) main.o
	$(CXX) $(CXXFLAGS) $^ -o $@

# Test executable
$(TEST_EXEC): $(TEST_OBJS) $(filter-out main.o, $(OBJS))
	$(CXX) $(CXXFLAGS) $^ -o $@
	./$(TEST_EXEC)

# Object compilation
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clang-tidy
tidy:
	clang-tidy $(SOURCES) $(HEADERS) -checks='*,-cppcoreguidelines-pro-bounds-pointer-arithmetic,-cppcoreguidelines-owning-memory' -- -std=c++17

# Run Valgrind
valgrind: $(GAME_EXEC) $(TEST_EXEC)
	valgrind --tool=memcheck $(VALGRIND_FLAGS) ./$(GAME_EXEC) 2>&1 | { egrep "lost| at " || true; }
	valgrind --tool=memcheck $(VALGRIND_FLAGS) ./$(TEST_EXEC) 2>&1 | { egrep "lost| at " || true; }

# Clean up command to remove all compiled files
clean:
	rm -f *.o $(GAME_EXEC) $(TEST_EXEC)
