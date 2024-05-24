#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>
#include <map>
#include <set>
#include <vector>
#include "board.hpp"
#include "resource.hpp"
#include "developmentCard.hpp"

using namespace std;
namespace ariel {

    class Player
    {
        private:
            static int nextID;
            string name;
            int id;
            map<ResourceType, int> resources;
            map<DevCardType, int> developmentCards;
            size_t points;

        public:

            // Constructor 
            Player(const string& name);
           
            // Map of all building costs
            static const map<string, map<ResourceType, int>> buildingCosts;

            // Checking methods
            bool canBuild(const string& structureType);
            void build(const string& structureType);

            // Building methods
            void placeSettlement(int intersectionID, Board& board);
            void placeRoad(const Edge& edge, Board& board);

            // Methods to manage resources
            void addResource(ResourceType type, int quantity);
            bool useResources(ResourceType type, int quantity);

            // Methods to manage development cards
            void buyDevelopmentCard(DevCardType type);
            void useDevelopmentCard(DevCardType type);
            
            // Trading method
            void trade(Player& other, const string& give, const string& receive, int giveAmount, int receiveAmount);

            // Rolls a single six-sided die.
            static int rollDice();
            void endTurn();

            int getPoints() const;
            string getName() const;
            int getId() const;
            void printPoints() const;
    };
}

#endif