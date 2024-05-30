#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>
#include <map>
#include <set>
#include <vector>
#include <random>
#include <ctime>
#include "board.hpp"
// #include "player.hpp"
#include "intersection.hpp"
// #include "resource.hpp"
#include "DevelopmentCard.hpp"
#include "edge.hpp"
#include "card_type.hpp"

using namespace std;
namespace ariel {
    class Board;

    enum class CardPurchaseError { Success, InsufficientResources, CardUnavailable };

    enum class CardUseError { Success, InsufficientCards, InvalidCardType };

    class Player
    {
        private:
            static int nextID;
            string name;
            int id;
            std::map<ResourceType, int> resources;
            std::map<DevCardType, int> developmentCards;
            std::map<PromotionType, int> promotionCards;
            set<int> settlements;   // Holds the intersection IDs where this player has settlements
            set<int> cities;
            set<Edge> roads;        // Holds the roads this player has built
            size_t points;
        

        public:

            // Constructor 
            Player(const string& name);
           
            // Map of all building costs
            static const map<string, map<ResourceType, int>> buildingCosts;
            static const map<ResourceType, int> devCardCosts;

            // Checking methods
            void placeInitialSettlement(int intersectionID, Board& board);
            void placeInitialRoad(const Edge& edge, Board& board);
            bool canBuild(const string& structureType);
            void buildRoad(const Edge& edge, Board& board);
            void buildSettlement(int intersectionID, Board& board);
            void upgradeToCity(int intersectionID, Board& board);
            void build(const string& structureType);

            // Building methods
            void placeSettlement(int intersectionID, Board& board);
            void placeRoad(const Edge& edge, Board& board);

            // Methods to manage resources
            void addResource(ResourceType type, int quantity);
            void printResources() const;
            bool useResources(ResourceType type, int quantity);
            int getResourceCount(ResourceType type) const;

            // Methods to manage development cards
            CardPurchaseError buyDevelopmentCard(DevCardType type);
            CardUseError useDevelopmentCard(DevCardType cardType, vector<Player*>& allPlayers, Board& board, bool& endTurn);
            void useKnightCard(); 
            void usePromotionCard(PromotionType type, vector<Player*>& allPlayers, Board& board);
            void useMonopoly(vector<Player*>& allPlayers);
            void useRoadBuilding(Board& board);
            bool isRoadContinuation(int id1, int id2);
            void useYearOfPlenty();
            ResourceType chooseResource();

            // Trading method
            void trade(Player& other, const string& give, const string& receive, int giveAmount, int receiveAmount);

            // Rolls a single six-sided die.
            static int rollDice();
            void endTurn();

            int getPoints() const;
            string getName() const;
            int getId() const;
            const set<int>& getCities() const { return cities; }
            void printPoints() const;
            void addPoints(int pointsToAdd);

            // New methods to access player's structures
            const set<int>& getSettlements() const { return settlements; }
            const set<Edge>& getRoads() const { return roads; }

            string printPlayer() const;
            void printCardCounts() const;


            
    };
}

#endif