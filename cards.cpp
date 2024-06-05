#include "cards.hpp"
#include <iostream>

using namespace std;
namespace ariel {

    // Initialize static quantities for each card type
    int KnightCard::quantity = 14;
    int VictoryPointCard::quantity = 4;
    int MonopolyCard::quantity = 2;
    int RoadBuildingCard::quantity = 2;
    int YearOfPlentyCard::quantity = 2;

    /**
     * @brief Returns the total quantity available for a specified type of development card.
     * @param type The type of development card.
     * @return The quantity of the specified card type.
     */
    int DevelopmentCard::getCardQuantity(DevCardType type) 
    {
        switch (type) 
        {
            case DevCardType::KNIGHT:
                return KnightCard::getQuantity();
            case DevCardType::VICTORY_POINT:
                return VictoryPointCard::getQuantity();
            case DevCardType::PROMOTION:
                return MonopolyCard::getQuantity() + RoadBuildingCard::getQuantity() + YearOfPlentyCard::getQuantity();
            default:
                return 0;
        }
    }


    /**
     * @brief Gets the type of the Knight card.
     * @return The type as a string.
     */    
    string KnightCard::getType() const 
    {
        return "Knight";
    }


    /**
     * @brief Returns the current quantity of Knight cards.
     * @return The quantity of Knight cards.
     */
    int KnightCard::getQuantity() 
    {
        return quantity;
    }


    /**
     * @brief Decreases the quantity of Knight cards by one.
     */
    void KnightCard::decreaseQuantity() 
    {
        if (quantity > 0) 
        {
            quantity--;
        }
    }

    /**
     * @brief Gets the type of the Victory Point card.
     * @return The type as a string.
     */   
    string VictoryPointCard::getType() const 
    {
        return "Victory Point";
    }

    /**
     * @brief Returns the current quantity of Victory Point cards.
     * @return The quantity of Victory Point cards.
     */
    int VictoryPointCard::getQuantity()
    {
        return quantity;
    }


    /**
     * @brief Decreases the quantity of Victory Point cards by one.
     */
    void VictoryPointCard::decreaseQuantity() 
    {
        if (quantity > 0) 
        {
            quantity--;
        }
    }


    /**
     * @brief Gets the type of the Monopoly card.
     * @return The type as a string.
     */
    string MonopolyCard::getType() const
    {
        return "Monopoly";
    }


    /**
     * @brief Returns the current quantity of Monopoly cards.
     * @return The quantity of Monopoly cards.
     */
    int MonopolyCard::getQuantity() 
    {
        return quantity;
    }


    /**
     * @brief Decreases the quantity of Monopoly cards by one.
     */
    void MonopolyCard::decreaseQuantity() 
    {
        if (quantity > 0) 
        {
            quantity--;
        }
    }


    /**
     * @brief Gets the type of the Road Building card.
     * @return The type as a string.
     */
    string RoadBuildingCard::getType() const 
    {
        return "Road Building";
    }


    /**
     * @brief Returns the current quantity of Road Building cards.
     * @return The quantity of Road Building cards.
     */
    int RoadBuildingCard::getQuantity() 
    {
        return quantity;
    }


    /**
     * @brief Decreases the quantity of Road Building cards by one.
     */
    void RoadBuildingCard::decreaseQuantity() 
    {
        if (quantity > 0) 
        {
            quantity--;
        }
    }

    /**
     * @brief Gets the type of the Year of Plenty card.
     * @return The type as a string.
     */
    string YearOfPlentyCard::getType() const 
    {
        return "Year of Plenty";
    }

    
    /**
     * @brief Returns the current quantity of Year of Plenty cards.
     * @return The quantity of Year of Plenty cards.
     */
    int YearOfPlentyCard::getQuantity()
    {
        return quantity;
    }


    /**
     * @brief Decreases the quantity of Year of Plenty cards by one.
     */
    void YearOfPlentyCard::decreaseQuantity() 
    {
        if (quantity > 0) 
        {
            quantity--;
        }
    }


    /**
     * @brief Converts a development card type to a string description.
     * @param type The development card type.
     * @return The string description of the card type.
     */
    string devCardTypeToString(DevCardType type) 
    {
        switch (type) 
        {
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


     /**
     * @brief Converts a promotion card type to a string description.
     * @param type The promotion card type.
     * @return The string description of the promotion type.
     */
    string promotionTypeToString(PromotionType type) 
    {
        switch (type) 
        {
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
