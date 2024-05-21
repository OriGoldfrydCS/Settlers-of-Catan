#include "developmentCard.hpp"

using namespace std;
namespace ariel {

    DevelopmentCard::DevelopmentCard(DevCardType type, const string& effect) : type(type), effect(effect) {}

    DevCardType DevelopmentCard::getType() const 
    {
        return this->type;
    }

    string DevelopmentCard::getEffect() const 
    {
        return this->effect;
    }

    PromotionCard::PromotionCard(PromotionType promotionType, const string& effect) : DevelopmentCard(DevCardType::PROMOTION, effect), promotionType(promotionType) {}

    void PromotionCard::useCard() 
    {
        // Implement the logic to use the Promotion card
    }

    KnightCard::KnightCard(int knightCount, const string& effect) : DevelopmentCard(DevCardType::KNIGHT, effect), knightCount(knightCount) {}

    void KnightCard::useCard() 
    {
        // Implement the logic to use the Knight card
    }

    VictoryPointCard::VictoryPointCard(const string& effect) : DevelopmentCard(DevCardType::VICTORY_POINT, effect) {}

    void VictoryPointCard::useCard() 
    {
        // Implement the logic to use the Victory Point card
    }
}