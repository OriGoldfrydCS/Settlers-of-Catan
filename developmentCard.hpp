#ifndef DEVELOPMENT_CARD_HPP
#define DEVELOPMENT_CARD_HPP

#include <string>
#include "card_type.hpp"

using namespace std;
namespace ariel {

    class DevelopmentCard 
    {
        private:
            DevCardType type;
            string effect;

        public:
            DevelopmentCard(DevCardType type, const string& effect);
            DevCardType getType() const;
            string getEffect() const;
            virtual void useCard() = 0;     
            
    };

    class PromotionCard : public DevelopmentCard 
    {
        private:
            PromotionType promotionType;

        public:
            PromotionCard(PromotionType promotionType, const string& effect);
            void useCard() override;
    };

    class KnightCard : public DevelopmentCard 
    {
        private:
            int knightCount;

        public:
            KnightCard(int knightCount, const string& effect);
            void useCard() override;
    };

    class VictoryPointCard : public DevelopmentCard 
    {
        public:
            VictoryPointCard(const string& effect);
            void useCard() override;
    };
}

#endif