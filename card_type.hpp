#ifndef CARD_TYPE_HPP
#define CARD_TYPE_HPP

#include <string>

namespace ariel {

    enum class DevCardType {
        PROMOTION,              // Promotion Cards
        KNIGHT,                 // Used to move the robber and steal resources; contributes to largest army
        VICTORY_POINT           // Grants one victory point immediately when drawn
    };

    enum class PromotionType {
        MONOPOLY,               // Player chooses a resource, all other players must give all of that resource to him
        ROAD_BUILDING,          // Player can build two roads at no cost
        YEAR_OF_PLENTY          // Player gets to receive two resource cards from the bank of his choice to use immediately
    };

    // Convert Development Card Type to string
    std::string devCardTypeToString(DevCardType type);

    // Convert Promotion Type to string
    std::string promotionTypeToString(PromotionType type);
};

#endif
