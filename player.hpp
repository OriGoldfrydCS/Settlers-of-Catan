// Email: origoldbsc@gmail.com

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>
#include <map>
#include <set>
#include <vector>
#include <random>
#include <sstream>
#include <functional>
#include <ctime>
#include "board.hpp"
#include "intersection.hpp"
#include "cards.hpp"
#include "edge.hpp"
#include "cards.hpp"

using namespace std;
namespace ariel {
    class Board;
    enum class DevCardType;
    enum class PromotionType;

    // Error types for attempting to purchase cards
    enum class CardPurchaseError { Success, InsufficientResources, CardUnavailable };

    // Error types for attempting to use cards
    enum class CardUseError { Success, InsufficientCards, InvalidCardType, Failure };

    /**
     * This class represents a player in the game.
     * Handles actions like building roads, settlements, trading resources, and playing development cards.
     */
    class Player
    {
        private:

            static int nextID;                            // Static counter to assign unique IDs to players
            string name;                                  // Player's name
            int id;                                       // Unique player ID
            map<ResourceType, int> resources;             // Resources owned by the player
            map<DevCardType, int> developmentCards;       // Development cards owned by the player
            map<PromotionType, int> promotionCards;       // Promotional cards owned by the player
            set<int> settlements;                         // Intersection IDs where the player has settlements
            set<int> cities;                              // Intersection IDs where the player has cities
            set<Edge> roads;                              // Edges where the player has built roads
            size_t points;                                // Player's victory points
            int knightCards;   

        public:

            static Player* largestArmyHolder;             // Static pointer to the player holding the largest army

            // Constructor 
            Player(const string& name);
           
            // Building and cards costs' maps
            static const map<string, map<ResourceType, int>> buildingCosts;
            static const map<ResourceType, int> devCardCosts;

            // Checking methods
            bool canBuild(const string& structureType);
            bool isRoadContinuation(int id1, int id2);

            // Building methods
            void placeInitialSettlement(int intersectionID, Board& board);
            void placeInitialRoad(const Edge& edge, Board& board);
            void buildSettlement(int intersectionID, Board& board);
            void buildRoad(const Edge& edge, Board& board);
            void upgradeToCity(int intersectionID, Board& board);

            // Methods to manage resources
            void addResource(ResourceType type, int quantity);
            bool useResources(ResourceType type, int quantity);
            int getResourceCount(ResourceType type) const;
            void printResources() const;

            // Methods to buy development cards
            CardPurchaseError buyDevelopmentCard(DevCardType type, vector<Player*>& allPlayers);
            bool hasEnoughResourcesForCard() const;
            CardPurchaseError handleCardPurchase(DevCardType cardType, vector<Player*>& allPlayers);
            CardPurchaseError handlePromotionCardPurchase();
            CardPurchaseError buySpecificPromotionCard(function<int()> getQuantity, function<void()> decreaseQuantity, PromotionType type);

            // Methods to use development cards
            CardUseError useDevelopmentCard(DevCardType cardType, vector<Player*>& allPlayers, Board& board, bool& endTurn);
            CardUseError handlePromotionCardUsage(vector<Player*>& allPlayers, Board& board, bool& endTurn);

            // Additional Methods regaring the Largest Army card
            void checkForLargestArmy(vector<Player*>& allPlayers);
            void reevaluateLargestArmy(vector<Player*>& allPlayers);

            
            // Handling resource discard after a dice roll of 7
            void discardResources(int toDiscard);

            // Trading method
            void trade(vector<Player*>& allPlayers);
            void tradeResources(vector<Player*>& allPlayers);
            Player* selectTradePartner(vector<Player*>& allPlayers);
            bool prepareTradeDetails(Player& recipient, map<ResourceType, int>& offerResources, map<ResourceType, int>& requestResources);
            void collectTradeDetails(const string& prompt, map<ResourceType, int>& details);
            void printTradeDetails(const map<ResourceType, int>& offer, const map<ResourceType, int>& request);
            void executeTrade(Player& offerer, Player& recipient, const map<ResourceType, int>& offerResources, const map<ResourceType, int>& requestResources);
            bool hasSufficientResources(const map<ResourceType, int>& resourcesNeeded);
            void tradeCards(vector<Player*>& allPlayers);
            bool prepareCardTradeDetails(Player& recipient, map<DevCardType, int>& offerCards, map<DevCardType, int>& requestCards);
            void collectCardTradeDetails(const string& prompt, map<DevCardType, int>& cardDetails);
            bool hasSufficientCards(const map<DevCardType, int>& cardsNeeded);
            void executeCardTrade(Player& offerer, Player& recipient, const map<DevCardType, int>& offerCards, const map<DevCardType, int>& requestCards, vector<Player*>& allPlayers);
            void printTradeDetails(const map<DevCardType, int>& offer, const map<DevCardType, int>& request);


            // Rolls a single six-sided die and end turn
            static int rollDice();
            void endTurn();

            // Getters & Setters 
            const map<DevCardType, int>& getDevelopmentCards() const;
            int getPromotionCardCount(PromotionType type) const;
            void setDevelopmentCardCount (DevCardType cardType, int count);
            void setPromotionCardCount(PromotionType type, int count);
            int getPoints() const;
            string getName() const;
            int getId() const;
            const set<int>& getCities() const;
            void addPoints(size_t pointsToAdd);
            const set<int>& getSettlements() const;
            const set<Edge>& getRoads() const;
            void addRoad(const Edge& edge);
    

            // Debugging and information display and other
            string printPlayer() const;
            void printCardCounts() const;
            void printPoints() const;
            int countTotalResources() const;


            // Methods modified for testing purposes
            void buildSettlementForTesting(int intersectionID, Board& board);
            bool hasBuiltRoad() const;
            bool hasBuiltSettlement() const;
    };
}

#endif