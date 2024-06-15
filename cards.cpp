// Email: origoldbsc@gmail.com

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

    //-------------------------------------//
    //  DevelopmentCard - Abstract Class   //
    //-------------------------------------//

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

    //-------------------------------------//
    //             KnightCard              //
    //-------------------------------------//

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

    /* @brief Activates the effect of a Knight card.
    *         Additional handling if it's a Knight card (not in the game's rule).
    */
    CardUseError KnightCard::activateCard(Player& player, vector<Player*>& allPlayers, Board& board, bool& endTurn) 
    {
        // Optional for future
        return CardUseError::Success;
    }

    //-------------------------------------//
    //         VictoryPointCard            //
    //-------------------------------------//

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

    /* @brief Activates the effect of a Victory Point card.
    * @param player Reference to the player using the card.
    * @param allPlayers Reference to a vector containing pointers to all players in the game.
    * @param board Reference to the Board object representing the game board.
    * @param endTurn Reference to a boolean that indicates whether the turn should end after using the card.
    * @return CardUseError An enum value that indicates the success or failure of the operation.
    */
    CardUseError VictoryPointCard::activateCard(Player& player, vector<Player*>& allPlayers, Board& board, bool& endTurn) 
    {
        player.addPoints(1);
        player.setDevelopmentCardCount(DevCardType::VICTORY_POINT, player.getDevelopmentCards().at(DevCardType::VICTORY_POINT) - 1);
        cout << player.getName() << " gained a victory point from a Victory Point Card." << endl;
        endTurn = true;
        return CardUseError::Success;
    }

    //-------------------------------------//
    //           MonopolyCard              //
    //-------------------------------------//

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
     * @brief Executes the Monopoly card's effect by allowing the current player to monopolize a chosen resource.
     * This function first checks if the player has a Monopoly card available. If so, it guide the player to
     * choose a resource type to monopolize. All other players must then give all their resources of that type
     * to the current player.
     * @param players A reference to the vector containing all players in the game.
     * @return CardUseError Indicates whether the Monopoly card was used successfully or not.
     */
    CardUseError MonopolyCard::activateCard(Player& player, vector<Player*>& allPlayers, Board& board, bool& endTurn) 
    {
        if (player.getPromotionCardCount(PromotionType::MONOPOLY) < 1) 
        {
            cout << "You do not have a Monopoly card to use." << endl;
            return CardUseError::InsufficientCards;
        }

        cout << "\nSelect the resource type to monopolize:\n1. Wood\n2. Brick\n3. Wool\n4. Grain\n5. Ore\nEnter your choice: ";
        int choice;
        cin >> choice;
        ResourceType chosenResource = static_cast<ResourceType>(choice - 1);

        int totalCollected = 0;
        for (auto& otherPlayer : allPlayers) 
        {
            // Ensure the current player is not taking resources from themselves
            if (otherPlayer->getId() != player.getId()) 
            { 
                int resourceAmount = otherPlayer->getResourceCount(chosenResource);
                if (resourceAmount > 0) 
                {
                    player.addResource(chosenResource, resourceAmount);             // Add the resources to the current player
                    otherPlayer->useResources(chosenResource, resourceAmount);      // Subtract the resources from other players
                    cout << otherPlayer->getName() << " gives " << resourceAmount << " " << resourceTypeToString(chosenResource) << " to " << player.getName() << "." << endl;
                    totalCollected += resourceAmount;
                }
            }
        }

        cout << player.getName() << " now has " << totalCollected << " more " << resourceTypeToString(chosenResource) << "." << endl;
        player.setPromotionCardCount(PromotionType::MONOPOLY, player.getPromotionCardCount(PromotionType::MONOPOLY) - 1); // Decrement the count of Monopoly cards
        return CardUseError::Success;
    }

    //-------------------------------------//
    //         RoadBuildingCard            //
    //-------------------------------------//

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
     * @brief Activates the effect of a Road Building card.
     * @param player Reference to the player using the card.
     * @param board Reference to the Board object representing the game board.
     * @param endTurn Reference to a boolean that indicates whether the turn should end after using the card.
     * @return CardUseError An enum value that indicates the success or failure of the operation.
     */
    CardUseError RoadBuildingCard::activateCard(Player& player, Board& board, bool& endTurn) 
    {
        cout << player.getName() << " uses Road Building to place two roads at no resource cost." << endl;
        int roadsToBuild = 2;
        while (roadsToBuild > 0) 
        {
            cout << "Enter the IDs of two intersections to place a road (1-54), separated by a space: ";
            int intersection1, intersection2;
            cin >> intersection1 >> intersection2;

            try {
                const Intersection& inter1 = Intersection::getIntersection(intersection1);
                const Intersection& inter2 = Intersection::getIntersection(intersection2);

                // Validate the placement for adjacency and connection to existing roads or settlements
                if (!board.areIntersectionsAdjacent(intersection1, intersection2) 
                || (!player.getSettlements().count(intersection1) && !player.getSettlements().count(intersection2) 
                && !player.isRoadContinuation(intersection1, intersection2))) 
                {
                    cout << "Invalid road placement: intersections are not adjacent or not connected to your settlements or existing roads." << endl;
                    continue;
                }

                // Create an edge and try to place the road
                Edge newRoad(inter1, inter2);

                if (!board.canPlaceRoad(newRoad, player.getId())) 
                {
                    cout << "Road cannot be placed between intersections " << intersection1 << " and " << intersection2
                        << ". It may already be occupied." << endl;
                    continue;       // Skip to the next iteration if the placement is invalid
                }

                // Place the road on the board and record the placement
                board.placeRoad(newRoad, player.getId());
                player.addRoad(newRoad);
                cout << "Road successfully built between intersections " << intersection1 << " and " << intersection2 << "." << endl;
                roadsToBuild--;      // Decrement the count of roads left to build
            } 
            catch (const out_of_range& e) 
            {
                cout << "Invalid intersection IDs provided. " << e.what() << endl;
                continue;
            }
        }

        player.setPromotionCardCount(PromotionType::ROAD_BUILDING, player.getPromotionCardCount(PromotionType::ROAD_BUILDING) - 1);
        return roadsToBuild == 0 ? CardUseError::Success : CardUseError::Failure;
    }

    //-------------------------------------//
    //         YearOfPlentyCard            //
    //-------------------------------------//

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
     * @brief Allows the player to make additional actions granted by the 'Year of Plenty' card.
     * @param player Reference to the player using the card.
     * @param board A reference to the Board object representing the game board.
     * @param endTurn Reference to a boolean that indicates whether the turn should end after using the card.
     * @return bool True if all actions are successfully executed, false otherwise.
     */
    CardUseError YearOfPlentyCard::activateCard(Player& player, Board& board, bool& endTurn) 
    {
        cout << player.getName() << " is using a 'Year of Plenty' card." << endl;

        cout << player.getName() << " is using a 'Year of Plenty' card." << endl;

        // Allow the player to select two resources from the bank
        ResourceType firstResource = chooseResource(player, "Choose the first resource to receive:");
        ResourceType secondResource = chooseResource(player, "Choose the second resource to receive:");
        player.addResource(firstResource, 1);
        player.addResource(secondResource, 1);

        // Display the resources received and update inventory
        cout << player.getName() << " received one unit each of " << resourceTypeToString(firstResource)
            << " and " << resourceTypeToString(secondResource) << " from the bank." << endl;
        player.printResources();

        // Use additional actions function per the Year of Plenty card
        cout << "You may now take additional actions with your new resources." << endl;
        
        int actionCount = 2;  // Allowing two actions as per the card's benefit
        bool allActionsSuccess = true;
        
        while (actionCount > 0) 
        {
            cout << "You have " << actionCount << " actions remaining." << endl;
            int choice = promptActionChoice(player);
            if (executeAction(choice, player, board, actionCount)) 
            {
                actionCount--;  // Decrease the count only if an action was successfully executed
            } 
            else 
            {
                allActionsSuccess = false;  // If an action fails, note the failure
            }
        }
        cout << "All actions completed or skipped." << endl;

        player.setPromotionCardCount(PromotionType::YEAR_OF_PLENTY, player.getPromotionCardCount(PromotionType::YEAR_OF_PLENTY) - 1);
        return allActionsSuccess ? CardUseError::Success : CardUseError::Failure;
    }

    /**
     * @brief Guide the player to choose a resource type from a list of available options.
     * @param player Reference to the player making the choice.
     * @param prompt A string containing the message to display to the player.
     * @return ResourceType The type of resource chosen by the player or the default resource if an invalid choice is made.
     */
    ResourceType YearOfPlentyCard::chooseResource(const Player& player, const string& prompt) {
        cout << prompt << endl;
        cout << "1. Wood\n2. Brick\n3. Wool\n4. Grain\n5. Ore\nEnter your choice: ";
        int choice;
        cin >> choice;
        switch (choice) {
            case 1: return ResourceType::WOOD;
            case 2: return ResourceType::BRICK;
            case 3: return ResourceType::WOOL;
            case 4: return ResourceType::GRAIN;
            case 5: return ResourceType::ORE;
            default:
                cout << "Invalid choice, defaulting to Wood." << endl;
                return ResourceType::WOOD;
        }
    }

    /**
     * @brief Guide the player to choose an action from a predefined list of options.
     * @param player Reference to the player making the choice.
     * @return int The numerical choice corresponding to the action the player wishes to make.
     */
    int YearOfPlentyCard::promptActionChoice(const Player& player) 
    {
        cout << "Choose an action:\n";
        cout << "1. Build a road\n";
        cout << "2. Build a settlement\n";
        cout << "3. Upgrade to a city\n";
        cout << "4. Pass (skip action)\n";
        cout << "Enter your choice: ";
        int choice;
        cin >> choice;
        return choice;
    }

    /**
     * @brief Executes an action based on the player's choice during their turn of Year of Plenty card use.
     * @param choice The player's choice.
     * @param player Reference to the player executing the action.
     * @param board Reference to the game board where the actions are to be applied.
     * @param actionCount Reference to the count of remaining actions.
     * @return bool True if the action is executed successfully, false otherwise.
     */
    bool YearOfPlentyCard::executeAction(int choice, Player& player, Board& board, int& actionCount) 
    {
        switch (choice) 
        {
            case 1:  // Build a road
                return attemptToBuildRoad(player, board);
            case 2:  // Build a settlement
                return attemptToBuildSettlement(player, board);
            case 3:  // Upgrade to a city
                return attemptToUpgradeToCity(player, board);
            case 4:  // Pass
                cout << "Skipping action." << endl;
                actionCount--;
                return true;        // Always "succeed" at skipping
            default:
                cout << "Invalid choice. Please choose a valid action." << endl;
                return false;
        }
    }

    /**
     * @brief Attempts to build a road on the game board based on player input for intersections.
     * @param player Reference to the player building the road.
     * @param board Reference to the game board.
     * @return bool True if the road is built successfully, false if the placement is invalid or if resources are insufficient.
     */
    bool YearOfPlentyCard::attemptToBuildRoad(Player& player, Board& board)
    {
        cout << "Enter the intersection IDs to place a road (e.g., 4 5): ";
        int intersectionID1, intersectionID2;
        cin >> intersectionID1 >> intersectionID2;
        Edge edge(Intersection::getIntersection(intersectionID1), Intersection::getIntersection(intersectionID2));
        if (player.canBuild("road") && board.canPlaceRoad(edge, player.getId())) 
        {
            player.buildRoad(edge, board);
            cout << "Road built successfully." << endl;
            return true;
        }
        cout << "Failed to build road. Check if the road is valid or if you have enough resources." << endl;
        return false;
    }

    /**
     * @brief Attempts to place a settlement at a specified location on the board.
     * @param player Reference to the player building the settlement.
     * @param board Reference to the game board.
     * @return bool True if the settlement is built successfully, false if the location is invalid or resources are insufficient.
     */
    bool YearOfPlentyCard::attemptToBuildSettlement(Player& player, Board& board) 
    {
        cout << "Enter the intersection ID to place a settlement: ";
        int intersectionID;
        cin >> intersectionID;
        if (player.canBuild("settlement") && board.canPlaceSettlement(intersectionID, player.getId())) 
        {
            player.buildSettlement(intersectionID, board);
            cout << "Settlement built successfully." << endl;
            return true;
        }
        cout << "Failed to build settlement. Ensure you have enough resources and a valid location." << endl;
        return false;
    }

    /**
     * @brief Attempts to upgrade an existing settlement to a city at a specified location on the board.
     * @param player Reference to the player upgrading the settlement.
     * @param board Reference to the game board.
     * @return bool True if the city is upgraded successfully, false if the conditions are not met or resources are insufficient.
     */
    bool YearOfPlentyCard::attemptToUpgradeToCity(Player& player, Board& board) 
    {
        cout << "Enter the intersection ID to upgrade to a city: ";
        int intersectionID;
        cin >> intersectionID;
        if (player.canBuild("city") && board.canUpgradeSettlementToCity(intersectionID, player.getId())) 
        {
            player.upgradeToCity(intersectionID, board);
            cout << "City upgraded successfully." << endl;
            return true;
        }
        cout << "Failed to upgrade to a city. Ensure there is a settlement at the location and you have sufficient resources." << endl;
        return false;
    }

    //-------------------------------------//
    //          General methods            //
    //-------------------------------------//

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
