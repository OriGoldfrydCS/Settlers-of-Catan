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

            // Methods to buy development cards (*)
            void purchaseSelectedCard(DevCardType cardType, vector<Player*>& allPlayers);   // Related to buyDevelopmentCard()
            bool hasEnoughResourcesForCard() const;                                         // as above

            // Methods to use development cards (!)
            CardUseError handlePromotionCardUsage(Player* currentPlayer, vector<Player*>& allPlayers, Board& board, bool& endTurn);  // Related to useDevelopmentCard()
            
            // Trading methods (&)
            void tradeResources(vector<Player*>& allPlayers);
            Player* selectTradePartner(vector<Player*>& allPlayers);
            bool prepareTradeDetails(Player& recipient, map<ResourceType, int>& offerResources, map<ResourceType, int>& requestResources);
            void collectTradeDetails(const string& prompt, map<ResourceType, int>& details);
            void printTradeResourcesDetails(const map<ResourceType, int>& offer, const map<ResourceType, int>& request);
            void executeTrade(Player& offerer, Player& recipient, const map<ResourceType, int>& offerResources, const map<ResourceType, int>& requestResources);
            bool hasSufficientResources(const map<ResourceType, int>& resourcesNeeded);
            void tradeCards(vector<Player*>& allPlayers);
            bool prepareCardTradeDetails(Player& recipient, map<DevCardType, int>& offerCards, map<DevCardType, int>& requestCards);
            void collectCardTradeDetails(const string& prompt, map<DevCardType, int>& cardDetails);
            bool hasSufficientCards(const map<DevCardType, int>& cardsNeeded);
            void executeCardTrade(Player& offerer, Player& recipient, const map<DevCardType, int>& offerCards, const map<DevCardType, int>& requestCards, vector<Player*>& allPlayers);
        
        public:

            static Player* largestArmyHolder;             // Static pointer to the player holding the largest army

            // Constructor 
            Player(const string& name);
           
            // Building and cards costs' maps
            static const map<string, map<ResourceType, int>> buildingCosts;
            static const map<ResourceType, int> devCardCosts;

            // Checking methods
            bool canBuild(const string& structureType);           // Called from catan.cpp
            bool isRoadContinuation(int id1, int id2);            // Called from card.cpp

            // Building methods
            void placeInitialSettlement(int intersectionID, Board& board);  // Called from board.cpp
            void placeInitialRoad(const Edge& edge, Board& board);          // Called from board.cpp
            void buildSettlement(int intersectionID, Board& board);     // TO-DO: move to private section after presentation
            void buildRoad(const Edge& edge, Board& board);             // as above
            void upgradeToCity(int intersectionID, Board& board);       // as above

            // Methods to manage resources
            void addResource(ResourceType type, int quantity);          // TO-DO: move to private section after presentation
            bool useResources(ResourceType type, int quantity);         // as above
            int getResourceCount(ResourceType type) const;
            void printResources() const;

            // Methods to buy development cards (*)
            CardPurchaseError buyDevelopmentCard(vector<Player*>& allPlayers);      // Called from catan.cpp

            // Methods to use development cards (!)
            CardUseError useDevelopmentCard(DevCardType cardType, Player* currentPlayer, vector<Player*>& allPlayers, Board& board, bool& endTurn);  // Called from catan.cpp

            // Handling resource discard after a dice roll of 7
            void discardResources(int toDiscard);                       // Called from catan.cpp

            // Methods to handle the Largest Army award ($)
            void checkForLargestArmy(vector<Player*>& allPlayers);      // Called from test.cpp
            void reevaluateLargestArmy(vector<Player*>& allPlayers);    // Called from test.cpp  

            // Trading method (&)
            void trade(vector<Player*>& allPlayers);
            void printTradeCardsDetails(const map<DevCardType, int>& offer, const map<DevCardType, int>& request);

            // Rolls a single six-sided die and end turn
            static int rollDice();      // Called from catan.cpp
            void endTurn();             // Called from catan.cpp

            // Getters & Setters 
            const map<DevCardType, int>& getDevelopmentCards() const;
            int getPromotionCardCount(PromotionType type) const;
            void setDevelopmentCardCount (DevCardType cardType, int count);     // Called from card.cpp
            void setPromotionCardCount(PromotionType type, int count);          // Called from card.cpp
            int getPoints() const;
            string getName() const;
            int getId() const;
            const set<int>& getCities() const;
            void addPoints(size_t pointsToAdd);
            const set<int>& getSettlements() const;
            const set<Edge>& getRoads() const;
            void addRoad(const Edge& edge);         // Called from card.cpp

            // Debugging and information display and other
            string printPlayer() const;
            void printCardCounts() const;
            void printPoints() const;
            int countTotalResources() const;

            // Methods modified for testing purposes
            void buildSettlementForTesting(int intersectionID, Board& board);
            bool hasBuiltRoad() const;
            bool hasBuiltSettlement() const;
            CardPurchaseError buyDevelopmentCardTEST(DevCardType type, vector<Player*>& allPlayers);
            bool hasEnoughResourcesForCardTEST() const;
            CardPurchaseError handleCardPurchaseTEST(DevCardType cardType, vector<Player*>& allPlayers);
            CardPurchaseError handlePromotionCardPurchaseTEST();
            CardPurchaseError buySpecificPromotionCardTEST(function<int()> getQuantity, function<void()> decreaseQuantity, PromotionType type);
    };
}

#endif