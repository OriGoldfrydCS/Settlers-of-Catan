// Email: origoldbsc@gmail.com

#ifndef BOARD_HPP
#define BOARD_HPP

#include <map>
#include <set>
#include <utility>
#include "tile.hpp"
#include "intersection.hpp"
#include "edge.hpp"
#include "player.hpp"

namespace ariel {
    class Player;
    class Tile;
    
    /**
     * @brief This class represents the game board, managing tiles, intersections, and player constructions like roads and settlements.
     */
    class Board 
    {
        private:

            // Private constructors since board is singelton
            Board();

            map<pair<int, int>, Tile> tiles;                // Maps tiles by their grid coordinates
            map<int, std::set<int>> settlements;            // Tracks which player has settlements at each vertex
            std::map<int, int> cities;                      // Maps intersection IDs to player IDs for cities
            map<Edge, int> roads;                           // Maps edges (roads) to player IDs
            static map<int, std::set<int>> adjacencyList;   // Adjacency list for all intersections for placing a valid road

        public:

            // Static method to access the singleton instance
            static Board& getInstance();

            // Game board setup methods
            void setupTiles();
            void linkTilesAndIntersections(); 
            static void initializeAdjacency();


            // Tile and board display methods
            const Tile& getTile(const pair<int, int>& position) const;
            void printBoard() const;
            void printGameBoard() const;
            void printBoardByTiles() const;
            void printAllBoardToCheckTileIntersections() const;
            string tileString(const std::pair<int, int>& position) const;
            void addTile(const pair<int, int>& position, const Tile& tile);
            bool isTileAvailable(const pair<int, int>& position) const;

            // Settlement and road management methods
            void placeInitialSettlement(int intersectionID, int playerID);
            void placeInitialRoad(const Edge& edge, int playerID);
            bool canPlaceSettlement(int intersectionID, int playerID);
            bool isIntersectionConnectedToPlayerRoad(int intersectionID, int playerID);
            void placeSettlement(int intersectionID, int playerID);
            bool canPlaceRoad(const Edge& edge, int playerID);
            void placeRoad(const Edge& edge, int playerID);
            bool canUpgradeSettlementToCity(int intersectionID, int playerID);
            void upgradeSettlementToCity(int intersectionID, int playerID);

            // Resource management and utility methods
            int getIntersectionID(const Intersection& intersection) const;
            vector<ResourceType> getResourceTypesAroundIntersection(int intersectionID);
            bool areIntersectionsAdjacent(int id1, int id2);
            void distributeResourcesBasedOnDiceRoll(int diceRoll, const std::vector<Player*>& players);
            bool hasSettlement(int intersectionID);
            vector<Tile> getTilesAroundIntersection(int intersectionID) const;
            const map<int, int>& getCities() const;

            // Functions for tests
            bool isRoadPresent(int intersectionID1, int intersectionID2) const;
    };
}

#endif