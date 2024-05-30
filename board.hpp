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
    class Board 
    {
        private:
            map<pair<int, int>, Tile> tiles;            // Map of tiles by their coordinates
            map<int, std::set<int>> settlements;        // Tracks settlements at vertices by player ID
            std::map<int, int> cities;                  // Cities mapped by intersection IDs to player IDs
            map<Edge, int> roads;                       // Tracks roads along edges by player ID
            // map<int, Intersection> intersections;       // Map of all valid intersections
            static map<int, std::set<int>> adjacencyList;      // adjacencyList for all intersection for placing a valid road

        public:

            // Constructor and setup
            Board();
            void setupTiles();
            void linkTilesAndIntersections(); 
            static void initializeAdjacency();

            // Setup methods for GUI
            // void loadResources();
            // void draw(sf::RenderWindow& window);

            // Utility methods
            const Tile& getTile(const pair<int, int>& position) const;
            void printBoard() const;
            void printBoardByTiles() const;
            void printAllBoardToCheckTileIntersections() const;
            string tileString(const std::pair<int, int>& position) const;
            void addTile(const pair<int, int>& position, const Tile& tile);
            bool isTileAvailable(const pair<int, int>& position) const;

            // Settlement and road management
            void placeInitialSettlement(int intersectionID, int playerID);
            void placeInitialRoad(const Edge& edge, int playerID);
            bool canPlaceSettlement(int intersectionID, int playerID);
            bool isIntersectionConnectedToPlayerRoad(int intersectionID, int playerID);
            void placeSettlement(int intersectionID, int playerID);
            bool canPlaceRoad(const Edge& edge, int playerID);
            void placeRoad(const Edge& edge, int playerID);
            bool canUpgradeSettlementToCity(int intersectionID, int playerID);
            void upgradeSettlementToCity(int intersectionID, int playerID);

            // Get intersection by ID
            // Intersection getIntersection(int intersectionID) const;
            int getIntersectionID(const Intersection& intersection) const;


            vector<ResourceType> getResourceTypesAroundIntersection(int intersectionID);
            bool areIntersectionsAdjacent(int id1, int id2);
            void distributeResourcesBasedOnDiceRoll(int diceRoll, const std::vector<Player*>& players);
            bool hasSettlement(int intersectionID);
            vector<Tile> getTilesAroundIntersection(int intersectionID) const;

        

           
    };
}

#endif