#ifndef BOARD_HPP
#define BOARD_HPP

#include <map>
#include <set>
#include <utility>
#include "tile.hpp"
#include "vertex_edge.hpp"

namespace ariel {
    
    class Board 
    {
        private:
            map<pair<int, int>, Tile> tiles;
            map<Vertex, bool> settlements;
            map<Edge, bool> roads;

        public:
            Board();
            void setupTiles();

            const Tile& getTile(const pair<int, int>& position) const;
            void printBoard() const;
            void addTile(const pair<int, int>& position, const Tile& tile);
            bool isTileAvailable(const pair<int, int>& position) const;

            // Settlement and road management
            bool canPlaceSettlement(const set<Vertex>& vertices);
            void placeSettlement(const set<Vertex>& vertices);
            bool canPlaceRoad(const Edge& edge);
            void placeRoad(const Edge& edge);

            // Board();
            
            // void addTile(Tile* tile);
            // bool isTileAvailable(const string& type, int number) const;
            // void printBoard() const;
    };
}

#endif