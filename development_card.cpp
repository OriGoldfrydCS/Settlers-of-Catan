#include "development_card.hpp"
#include <iostream>

namespace ariel {

    int KnightCard::quantity = 14;
    int VictoryPointCard::quantity = 4;
    int MonopolyCard::quantity = 2;
    int RoadBuildingCard::quantity = 2;
    int YearOfPlentyCard::quantity = 2;

    int DevelopmentCard::getCardQuantity(DevCardType type) 
    {
        switch (type) {
            case DevCardType::KNIGHT:
                return KnightCard::getQuantity();
            case DevCardType::VICTORY_POINT:
                return VictoryPointCard::getQuantity();
            case DevCardType::PROMOTION:  // Assume an aggregate quantity for all promotion cards
                return MonopolyCard::getQuantity() + RoadBuildingCard::getQuantity() + YearOfPlentyCard::getQuantity();
            default:
                return 0;
        }
    }

    void KnightCard::decreaseQuantity() {
        if (quantity > 0) {
            quantity--;
        }
    }

    // void KnightCard::activate(Player& player, Board& board) {
    //     // Implementation of knight card activation
    // }

    std::string KnightCard::getType() const {
        return "Knight";
    }

    std::string KnightCard::getEffect() const {
        return "Move the robber and steal resources; may grant largest army.";
    }

    int KnightCard::getQuantity() {
        return quantity;
    }

    void VictoryPointCard::decreaseQuantity() {
        if (quantity > 0) {
            quantity--;
        }
    }

    void VictoryPointCard::activate(Player& player, Board& board) {
        // Implementation of victory point card activation
    }

    std::string VictoryPointCard::getType() const {
        return "Victory Point";
    }

    std::string VictoryPointCard::getEffect() const {
        return "Grants one victory point immediately when drawn.";
    }

    int VictoryPointCard::getQuantity() {
        return quantity;
    }

    void MonopolyCard::activate(Player& player, Board& board) {
        // Implementation of monopoly card activation
    }

    std::string MonopolyCard::getType() const {
        return "Monopoly";
    }

    std::string MonopolyCard::getEffect() const {
        return "Choose a resource type; all other players must give you all of their resources of that type.";
    }

    int MonopolyCard::getQuantity() {
        return quantity;
    }

    void MonopolyCard::decreaseQuantity() {
        if (quantity > 0) {
            quantity--;
        }
    }

    void RoadBuildingCard::decreaseQuantity() {
        if (quantity > 0) {
            quantity--;
        }
    }
    void RoadBuildingCard::activate(Player& player, Board& board) {
        // Implementation of road building card activation
    }

    std::string RoadBuildingCard::getType() const {
        return "Road Building";
    }

    std::string RoadBuildingCard::getEffect() const {
        return "Build two roads at no cost anywhere on the map.";
    }

    int RoadBuildingCard::getQuantity() {
        return quantity;
    }

    void YearOfPlentyCard::activate(Player& player, Board& board) {
        // Implementation of year of plenty card activation
    }

    std::string YearOfPlentyCard::getType() const {
        return "Year of Plenty";
    }

    std::string YearOfPlentyCard::getEffect() const {
        return "Receive two resource cards of your choice from the bank to use immediately.";
    }

    int YearOfPlentyCard::getQuantity() {
        return quantity;
    }

    void YearOfPlentyCard::decreaseQuantity() {
        if (quantity > 0) {
            quantity--;
        }
    }
}
