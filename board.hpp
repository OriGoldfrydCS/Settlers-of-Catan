#ifndef BOARD_HPP
#define BOARD_HPP

#include <map>
#include <utility>
#include "tile.hpp"

namespace ariel {
    
    class Board 
    {
        private:
            map<pair<int, int>, Tile> tiles;

        public:
            Board();
            void setupTiles();

            const Tile& Board::getTile(const std::pair<int, int>& position) const;
            void printBoard() const;
            void addTile(const std::pair<int, int>& position, const Tile& tile);
            bool isTileAvailable(const std::pair<int, int>& position) const;

            // Board();
            
            // void addTile(Tile* tile);
            // bool isTileAvailable(const string& type, int number) const;
            // void printBoard() const;
    };
}

#endif