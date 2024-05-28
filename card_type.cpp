#include "card_type.hpp"

namespace ariel {

    std::string devCardTypeToString(DevCardType type) {
        switch (type) {
            case DevCardType::PROMOTION:
                return "Promotion";
            case DevCardType::KNIGHT:
                return "Knight";
            case DevCardType::VICTORY_POINT:
                return "Victory Point";
            default:
                return "Unknown";
        }
    }

    std::string promotionTypeToString(PromotionType type) {
        switch (type) {
            case PromotionType::MONOPOLY:
                return "Monopoly";
            case PromotionType::ROAD_BUILDING:
                return "Road Building";
            case PromotionType::YEAR_OF_PLENTY:
                return "Year of Plenty";
            default:
                return "Unknown";
        }
    }
}
