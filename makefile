CXX=g++
CXXFLAGS=-std=c++17 -Wall -g
#SFMLFLAGS=-lsfml-graphics -lsfml-window -lsfml-system

# List all object files
OBJS=main.o board.o player.o tile.o catan.o resource_type.o  # Add resource_type.o if you go with this option

# Target to build the game
all: Catan

Catan: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) 

# Pattern rule for compiling source files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Target to clean the build
clean:
	rm -f $(OBJS) *.d Catan
