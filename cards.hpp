// Email: origoldbsc@gmail.com

#ifndef CARDS_HPP
#define CARDS_HPP

#include "player.hpp"
#include <string>
#include <vector>

using namespace std;
namespace ariel {

    class Player;
    class Board;
    enum class CardUseError;
    enum ResourceType;

    /**
     * @brief Enum for different types of development cards in the game.
     */    
    enum class DevCardType {
        PROMOTION,              // Promotion Cards
        KNIGHT,                 // Used to move the robber and steal resources; contributes to largest army
        VICTORY_POINT           // Grants one victory point immediately when drawn
    };


    /**
     * @brief Enum for different types of promotional cards in the game.
     */
    enum class PromotionType {
        MONOPOLY,               // Player chooses a resource, all other players must give all of that resource to him
        ROAD_BUILDING,          // Player can build two roads at no cost
        YEAR_OF_PLENTY          // Player gets to receive two resource cards from the bank of his choice to use immediately
    };

    //-------------------------------------//
    //  DevelopmentCard - Abstract Class   //
    //-------------------------------------//

    /**
     * @brief Abstract class for Development Cards.
     */
    class DevelopmentCard {
    public:
        virtual string getType() const = 0;
        static int getCardQuantity(DevCardType type);
    };


    //-------------------------------------//
    //             KnightCard              //
    //-------------------------------------//

    /**
     * @brief Represents a Knight Card in the game.
     */
    class KnightCard : public DevelopmentCard {
    private:
        static int quantity;
    public:
        string getType() const override;
        static int getQuantity();
        static void decreaseQuantity();
        CardUseError activateCard(Player& player, vector<Player*>& allPlayers, Board& board, bool& endTurn);
    };


    //-------------------------------------//
    //         VictoryPointCard            //
    //-------------------------------------//

    /**
     * @brief Represents a Victory Point Card in the game.
     */
    class VictoryPointCard : public DevelopmentCard {
    private:
        static int quantity;
    public:
        string getType() const override;
        static int getQuantity();
        static void decreaseQuantity();
        CardUseError activateCard(Player& player, vector<Player*>& allPlayers, Board& board, bool& endTurn);

    };


    //-------------------------------------//
    //    PromotionCard - Abstract Class   //
    //-------------------------------------//

    /**
     * @brief Abstract base class for Promotion Cards.
     */
    class PromotionCard : public DevelopmentCard {
    
    };


    //-------------------------------------//
    //           MonopolyCard              //
    //-------------------------------------//

    /**
     * @brief Represents a Monopoly Card in the game.
     */
    class MonopolyCard : public PromotionCard {
    private:
        static int quantity;
    public:
        string getType() const override;
        static int getQuantity();
        static void decreaseQuantity();
        CardUseError activateCard(Player& player, vector<Player*>& allPlayers, Board& board, bool& endTurn);

    };


    //-------------------------------------//
    //         RoadBuildingCard            //
    //-------------------------------------//

    /**
     * @brief Represents a Road Building Card in the game.
     */
    class RoadBuildingCard : public PromotionCard {
    private:
        static int quantity;
    public:
        string getType() const override;
        static int getQuantity();
        static void decreaseQuantity();
        CardUseError activateCard(Player& player, Board& board, bool& endTurn);
    };


    //-------------------------------------//
    //         YearOfPlentyCard            //
    //-------------------------------------//

    /**
     * @brief Represents a Year of Plenty Card in the game.
     */
    class YearOfPlentyCard : public PromotionCard {
    private:
        static int quantity;
    public:
        string getType() const override;
        static int getQuantity();
        static void decreaseQuantity();
        CardUseError activateCard(Player& player, Board& board, bool& endTurn);
        static ResourceType chooseResource(const Player& player, const string& prompt);
        static int promptActionChoice(const Player& player);
        static bool executeAction(int choice, Player& player, Board& board, int& actionCount);
        static bool attemptToBuildRoad(Player& player, Board& board);
        static bool attemptToBuildSettlement(Player& player, Board& board);
        static bool attemptToUpgradeToCity(Player& player, Board& board);
    };


    /**
     * @brief Converts a development card type to its string representation.
     * @param type The development card type.
     * @return The string description of the card type.
     */
    string devCardTypeToString(DevCardType type);
    

    /**
     * @brief Converts a promotion type to its string representation.
     *
     * @param type The promotion type.
     * @return The string description of the promotion type.
     */
    string promotionTypeToString(PromotionType type);
};

#endif 
