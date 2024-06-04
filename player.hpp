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
#include "development_card.hpp"
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
            int knightCards = 0;  // Track the number of Knight cards

        public:
            static Player* largestArmyHolder;

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
            CardPurchaseError buyDevelopmentCard(DevCardType type, vector<Player*>& allPlayers);
            CardUseError useDevelopmentCard(DevCardType cardType, vector<Player*>& allPlayers, Board& board, bool& endTurn);
            // void useKnightCard(); 
            void usePromotionCard(PromotionType type, vector<Player*>& allPlayers, Board& board);
            void useMonopoly(vector<Player*>& allPlayers);
            void useRoadBuilding(Board& board);
            bool isRoadContinuation(int id1, int id2);
            void useYearOfPlenty();
            ResourceType chooseResource(const string& prompt);
            void additionalActions(Board& board);
            void checkForLargestArmy(vector<Player*>& allPlayers);
            void reevaluateLargestArmy(vector<Player*>& allPlayers);

            
            // roll 7
            void discardResources(int toDiscard);
            ResourceType stringToResourceType(const string& input);

            // Trading method
            void trade(vector<Player*>& allPlayers);
            void collectTradeDetails(const std::string& prompt, std::map<ResourceType, int>& details);
            void printTradeDetails(const std::map<ResourceType, int>& offer, const std::map<ResourceType, int>& request);
            void executeTrade(Player& offerer, Player& recipient, const std::map<ResourceType, int>& offerResources, const std::map<ResourceType, int>& requestResources);
            bool hasSufficientResources(const std::map<ResourceType, int>& resourcesNeeded);
            void tradeCards(std::vector<Player*>& allPlayers);
            void collectCardTradeDetails(const string& prompt, map<DevCardType, int>& cardDetails);
            bool hasSufficientCards(const map<DevCardType, int>& cardsNeeded);
            void executeCardTrade(Player& offerer, Player& recipient, const map<DevCardType, int>& offerCards, const map<DevCardType, int>& requestCards, vector<Player*>& allPlayers);


            // Rolls a single six-sided die.
            static int rollDice();
            void endTurn();

            // Getters, Setters and 
            const map<DevCardType, int>& getDevelopmentCards() const;
            int getPoints() const;
            string getName() const;
            int getId() const;
            const set<int>& getCities() const { return cities; }
            void printPoints() const;
            void addPoints(size_t pointsToAdd);
            int countTotalResources() const;

            // New methods to access player's structures
            const set<int>& getSettlements() const { return settlements; }
            const set<Edge>& getRoads() const { return roads; }

            string printPlayer() const;
            void printCardCounts() const;


            // modified functions for tests////
            void buildSettlementForTesting(int intersectionID, Board& board);
            
    };
}

#endif