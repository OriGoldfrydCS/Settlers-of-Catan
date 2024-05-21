#ifndef BOARD_HPP
#define BOARD_HPP

#include <vector>
#include "tile.hpp"

using namespace std;
namespace ariel {
    
    class Board 
    {
        private:
            vector<Tile*> tiles;

        public:
            Board() {}
            
            void addTile(Tile* tile);
            bool isTileAvailable(const string& type, int number) const;
            void printBoard() const;
    };
}

#endif