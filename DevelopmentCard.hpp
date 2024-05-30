#ifndef DEVELOPMENT_CARD_HPP
#define DEVELOPMENT_CARD_HPP

#include <string>
#include "player.hpp"
#include "card_type.hpp"

namespace ariel {
    class Player;
    class Board;

    class DevelopmentCard {
    public:
        virtual ~DevelopmentCard() = default;
        virtual void activate(Player& player, Board& board) = 0;
        virtual std::string getType() const = 0;
        virtual std::string getEffect() const = 0;
        static int getCardQuantity(DevCardType type);

    };

    class KnightCard : public DevelopmentCard {
    private:
        static int quantity;

    public:
        KnightCard();
        void activate(Player& player, Board& board) override;
        std::string getType() const override;
        std::string getEffect() const override;
        static int getQuantity();
        static void decreaseQuantity();
    };

    class VictoryPointCard : public DevelopmentCard {
    private:
        static int quantity;

    public:
        VictoryPointCard();
        void activate(Player& player, Board& board) override;
        std::string getType() const override;
        std::string getEffect() const override;
        static int getQuantity();
        static void decreaseQuantity();

    };


    // Abstract class for Promotion Cards
    class PromotionCard : public DevelopmentCard {
    public:
        virtual ~PromotionCard() = default;
    };

    class MonopolyCard : public PromotionCard {
    private:
        static int quantity;

    public:
        MonopolyCard();
        void activate(Player& player, Board& board) override;
        std::string getType() const override;
        std::string getEffect() const override;
        static int getQuantity();
        static void decreaseQuantity();
    };

    class RoadBuildingCard : public PromotionCard {
    private:
        static int quantity;

    public:
        RoadBuildingCard();
        void activate(Player& player, Board& board) override;
        std::string getType() const override;
        std::string getEffect() const override;
        static int getQuantity();
        static void decreaseQuantity();

    };

    class YearOfPlentyCard : public PromotionCard {
    private:
        static int quantity;

    public:
        YearOfPlentyCard();
        void activate(Player& player, Board& board) override;
        std::string getType() const override;
        std::string getEffect() const override;
        static int getQuantity();
        static void decreaseQuantity();

    };
}

#endif
