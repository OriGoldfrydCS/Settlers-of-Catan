#include "player.hpp"
#include <iostream>
#include <sstream>
#include "card_type.hpp"

using namespace std;
namespace ariel {

    int Player::nextID = 1;     // Initialize the static member to 1
    Player* Player::largestArmyHolder = nullptr;

    Player::Player(const string& name) : name(name), id(nextID++), resources(), developmentCards(), points(0) { 
        for (auto& resource : {ResourceType::WOOD, ResourceType::BRICK, ResourceType::WOOL, ResourceType::GRAIN, ResourceType::ORE}) {
            resources[resource] = 0;
        }

        // Initialize promotion card types with zero count
        promotionCards[PromotionType::MONOPOLY] = 0;
        promotionCards[PromotionType::ROAD_BUILDING] = 0;
        promotionCards[PromotionType::YEAR_OF_PLENTY] = 0;

        // Initialize other development card types with zero count
        developmentCards[DevCardType::KNIGHT] = 0;
        developmentCards[DevCardType::VICTORY_POINT] = 0;
    }

    const map<string, map<ResourceType, int>> Player::buildingCosts = 
    {
        {"road", {{ResourceType::BRICK, 1}, {ResourceType::WOOD, 1}}},
        {"settlement", {{ResourceType::BRICK, 1}, {ResourceType::WOOD, 1}, {ResourceType::WOOL, 1}, {ResourceType::GRAIN, 1}}},
        {"city", {{ResourceType::ORE, 3}, {ResourceType::GRAIN, 2}}}
    };

    const std::map<ResourceType, int> Player::devCardCosts = {
        {ResourceType::ORE, 1},
        {ResourceType::WOOL, 1},
        {ResourceType::GRAIN, 1}
    };

    bool Player::canBuild(const string& structureType) 
    {
        const auto& cost = buildingCosts.at(structureType);
        for (const auto& resource : cost) {
            if (resources[resource.first] < resource.second)
                return false;
        }
        return true;
    }

    void Player::build(const string& structureType) {
        if (canBuild(structureType)) {
            const auto& cost = buildingCosts.at(structureType);
            for (const auto& resource : cost) {
                resources[resource.first] -= resource.second;
            }
            // Increment points accordingly
            if (structureType == "settlement") {
                points += 1;
            } else if (structureType == "city") {
                points += 1; // Assuming the player loses the settlement point
            }
        } else {
            throw runtime_error("Not enough resources to build " + structureType);
        }
    }

    void Player::placeInitialSettlement(int intersectionID, Board& board) {
        board.placeInitialSettlement(intersectionID, this->id);
        settlements.insert(intersectionID);  // Update the player's record of settlements
        addPoints(1);  // Each initial settlement should still count towards points
    }

    void Player::placeInitialRoad(const Edge& edge, Board& board) {
        board.placeInitialRoad(edge, this->id);
        this->roads.insert(edge);  // Update the player's record of roads
    }

    void Player::placeSettlement(int intersectionID, Board& board) {
        if (board.canPlaceSettlement(intersectionID, this->id)) {
            board.placeSettlement(intersectionID, this->id);
            settlements.insert(intersectionID);  // Record the placement of the settlement
            addPoints(1);
            // points += 1;  // Each settlement should give a point
        } 
    }

    void Player::addPoints(size_t pointsToAdd) {
        points = points + pointsToAdd;
    }

    void Player::buildRoad(const Edge& edge, Board& board) {
        if (canBuild("road") && board.canPlaceRoad(edge, this->id)) {
            resources[ResourceType::BRICK] -= 1;
            resources[ResourceType::WOOD] -= 1;
            roads.insert(edge);
            board.placeRoad(edge, this->id);
        }
    }

    void Player::buildSettlement(int intersectionID, Board& board) {
        if (canBuild("settlement") && board.canPlaceSettlement(intersectionID, this->id)) {
            resources[ResourceType::BRICK] -= 1;
            resources[ResourceType::WOOD] -= 1;
            resources[ResourceType::WOOL] -= 1;
            resources[ResourceType::GRAIN] -= 1;
            settlements.insert(intersectionID);
            board.placeSettlement(intersectionID, this->id);
            addPoints(1);
        }
    }

    void Player::upgradeToCity(int intersectionID, Board& board) {
        if (settlements.count(intersectionID) && canBuild("city")) {
            // Check if there are enough resources
            if (useResources(ResourceType::ORE, 3) && useResources(ResourceType::GRAIN, 2)) {
                // Remove from settlements and add to cities
                settlements.erase(intersectionID);
                cities.insert(intersectionID);
                
                // Perform the upgrade on the board as well
                board.upgradeSettlementToCity(intersectionID, this->id);
                
                // Adjust points (city gives 2 points, removing a settlement subtracts the 1 point it gave)
                addPoints(1);  // Net gain of 1 additional point
                
                cout << "Upgraded settlement at intersection " << intersectionID << " to a city." << endl;
            } else {
                cout << "Not enough resources to upgrade to a city." << endl;
            }
        } else {
            cout << "No settlement to upgrade at intersection " << intersectionID << endl;
        }
    }


    void Player::placeRoad(const Edge& edge, Board& board) 
    {
        if (board.canPlaceRoad(edge, this->id)) 
        {
            board.placeRoad(edge, this->id);
            roads.insert(edge);                                 // Record the placement of the road

        }
    }


    void placeCity(const string& type, int number, Board& board)
    {
        // Implement the logic to build a city
    }

    void Player::addResource(ResourceType type, int quantity) {
        resources[type] += quantity;
        // cout << "Added " << quantity << " " << resourceTypeToString(type) << " to Player " << id 
            // << ", total now: " << resources[type] << endl;
        // printResources(); // Calling to print all resources after updating

    }

    void Player::printResources() const {
        cout << name << "'s resources: ";
        for (const auto& [resourceType, amount] : resources) {
            cout << resourceTypeToString(resourceType) << ": " << amount << ", ";
        }
        cout << endl;
    }

    bool Player::useResources(ResourceType type, int quantity) 
    {
        if (resources[type] >= quantity) 
        {
            resources[type] -= quantity;
            return true;
        }
        return false;
    }

    CardPurchaseError Player::buyDevelopmentCard(DevCardType cardType, vector<Player*>& allPlayers){
        // Check if the player has enough resources
        if (resources[ResourceType::ORE] < 1 || resources[ResourceType::WOOL] < 1 
           || resources[ResourceType::GRAIN] < 1) 
        {
            return CardPurchaseError::InsufficientResources;
        }

        // Check availability of the card
        if (DevelopmentCard::getCardQuantity(cardType) <= 0) 
        {
            return CardPurchaseError::CardUnavailable;
        }

        // // Check if the player has enough resources
        // for (const auto& [resource, quantity] : devCardCosts) {
        //     if (resources[resource] < quantity) {
        //         return CardPurchaseError::InsufficientResources;;
        //     }
        // }

        // Deduct resources
        resources[ResourceType::ORE] -= 1;
        resources[ResourceType::WOOL] -= 1;
        resources[ResourceType::GRAIN] -= 1;


        // Deduct the card from the available pool and handle specific card purchase logic
        if (cardType == DevCardType::KNIGHT) {
            KnightCard::decreaseQuantity();
            developmentCards[DevCardType::KNIGHT]++;
            knightCards++; 
            checkForLargestArmy(allPlayers);  // Check if this player now qualifies for the Largest Army
            
        } else if (cardType == DevCardType::VICTORY_POINT) {
            VictoryPointCard::decreaseQuantity();
            developmentCards[DevCardType::VICTORY_POINT]++;
        } else if (cardType == DevCardType::PROMOTION) {
            // Allow player to select specific promotion card
            cout << "Enter 1 for Monopoly, 2 for Road Building, 3 for Year of Plenty: ";
            int choice;
            cin >> choice;

            switch (choice) {
                case 1:
                    if (MonopolyCard::getQuantity() > 0) {
                        MonopolyCard::decreaseQuantity();
                        promotionCards[PromotionType::MONOPOLY]++;
                        developmentCards[DevCardType::PROMOTION]++;
                    } else {
                        return CardPurchaseError::CardUnavailable;
                    }
                    break;
                case 2:
                    if (RoadBuildingCard::getQuantity() > 0) {
                        RoadBuildingCard::decreaseQuantity();
                        promotionCards[PromotionType::ROAD_BUILDING]++;
                        developmentCards[DevCardType::PROMOTION]++;
                    } else {
                        return CardPurchaseError::CardUnavailable;
                    }
                    break;
                case 3:
                    if (YearOfPlentyCard::getQuantity() > 0) {
                        YearOfPlentyCard::decreaseQuantity();
                        promotionCards[PromotionType::YEAR_OF_PLENTY]++;
                        developmentCards[DevCardType::PROMOTION]++;

                    } else {
                        return CardPurchaseError::CardUnavailable;
                    }
                    break;
                default:
                    cout << "Invalid choice, no card added." << endl;
                    return CardPurchaseError::CardUnavailable;
            }
        } else {
            cout << "Invalid card type." << endl;
            return CardPurchaseError::CardUnavailable;
        }

        // developmentCards[cardType]++;

        cout << "Card purchased: " << devCardTypeToString(cardType) << ". New count: " << developmentCards[cardType] << endl;

        return CardPurchaseError::Success;
    }


    void Player::checkForLargestArmy(vector<Player*>& allPlayers) {
        // Check if this player has the Largest Army
        if (knightCards >= 3) {
            if (largestArmyHolder == nullptr || largestArmyHolder->knightCards < knightCards) {
                if (largestArmyHolder != nullptr && largestArmyHolder != this) {
                    largestArmyHolder->points -= 2; // Subtract points from the previous holder
                    cout << largestArmyHolder->name << " has lost the Largest Army." << endl;
                }
                largestArmyHolder = this;
                points += 2;  // Award new points to this player
                cout << name << " now holds the Largest Army and gains 2 victory points." << endl;
            }
        } else {
            // Check if this player is currently holding the Largest Army but no longer qualifies
            if (largestArmyHolder == this) {
                largestArmyHolder = nullptr;
                points -= 2;
                cout << name << " has lost the Largest Army." << endl;
                // Re-evaluate the largest army holder among all players
                reevaluateLargestArmy(allPlayers);
            }
        }
    }

    // void Player::reevaluateLargestArmy(vector<Player*>& allPlayers) {
    //     Player* newHolder = nullptr;
    //     int maxKnights = 0;
    //     // This will require access to all players in the game, assuming you have a way to iterate over all players
    //     for (auto& player : allPlayers) {
    //         if (player->knightCards > maxKnights && player->knightCards >= 3) {
    //             maxKnights = player->knightCards;
    //             newHolder = player;
    //         }
    //     }
    //     if (newHolder != nullptr) {
    //         if (largestArmyHolder != nullptr) {
    //             largestArmyHolder->points -= 2; // Deduct points from the previous holder
    //         }
    //         largestArmyHolder = newHolder;
    //         largestArmyHolder->points += 2;  // Award points to the new holder
    //         cout << largestArmyHolder->name << " now holds the Largest Army and gains 2 victory points." << endl;
    //     }
    // }

    // void Player::reevaluateLargestArmy(vector<Player*>& allPlayers) {
    //     Player* currentHolder = Player::largestArmyHolder;
    //     int maxKnights = 0;
    //     Player* newHolder = nullptr;

    //     // Find the player with the highest number of knight cards who has at least 3
    //     for (Player* player : allPlayers) {
    //         if (player->developmentCards[DevCardType::KNIGHT] > maxKnights && player->developmentCards[DevCardType::KNIGHT] >= 3) {
    //             maxKnights = player->developmentCards[DevCardType::KNIGHT];
    //             newHolder = player;
    //         }
    //     }

    //     // Update the largest army holder if necessary
    //     if (newHolder != currentHolder) {
    //         if (currentHolder) {
    //             currentHolder->points -= 2; // Deduct points from previous holder
    //         }
    //         if (newHolder) {
    //             newHolder->points += 2; // Award points to new holder
    //         }
    //         Player::largestArmyHolder = newHolder;
    //     }
    // }

    void Player::reevaluateLargestArmy(vector<Player*>& allPlayers) {
        Player* currentHolder = Player::largestArmyHolder;
        int maxKnights = 0;
        Player* newHolder = nullptr;

        for (Player* player : allPlayers) {
            if (player->developmentCards[DevCardType::KNIGHT] > maxKnights) {
                maxKnights = player->developmentCards[DevCardType::KNIGHT];
                newHolder = player;
            }
        }

        if (newHolder && maxKnights >= 3 && newHolder != currentHolder) {
            if (currentHolder) {
                currentHolder->points -= 2;
            }
            Player::largestArmyHolder = newHolder;
            newHolder->points += 2;
            cout << newHolder->getName() << " now holds the Largest Army." << endl;
        } else if (!newHolder || maxKnights < 3) {
            Player::largestArmyHolder = nullptr; // No player qualifies for largest army
            if (currentHolder) {
                currentHolder->points -= 2;
            }
        }
    }




    CardUseError Player::useDevelopmentCard(DevCardType cardType, vector<Player*>& allPlayers, Board& board, bool& endTurn) 
    {
        // Print the attempt to use a card
        cout << "Attempting to use card type: " << devCardTypeToString(cardType) << endl;
        endTurn = false;        // Default to not ending the turn

        // First, handle the case where a promotion card is used
        if (cardType == DevCardType::PROMOTION) 
        {
            cout << "Promotion card type selected. Choose specific promotion card to use:" << endl;
            cout << "1. Monopoly\n2. Road Building\n3. Year of Plenty\nEnter your choice: ";
            int promoChoice;
            cin >> promoChoice;
            PromotionType promoType;
            switch (promoChoice) 
            {
                case 1:
                    promoType = PromotionType::MONOPOLY;
                    break;
                case 2:
                    promoType = PromotionType::ROAD_BUILDING;
                    break;
                case 3:
                    promoType = PromotionType::YEAR_OF_PLENTY;
                    break;
                default:
                    cout << "Invalid promotion type selected.\n";
                    return CardUseError::InvalidCardType;
            }

            if (promotionCards[promoType] < 1) {
                cout << "Insufficient " << promotionTypeToString(promoType) << " cards to use.\n";
                return CardUseError::InsufficientCards;
            }

            usePromotionCard(promoType, allPlayers, board);
            cout << "Used " << promotionTypeToString(promoType) << " card. Remaining: " << promotionCards[promoType] << endl;
            
            if (promoType == PromotionType::YEAR_OF_PLENTY) {
            additionalActions(board); // Call additional actions specific to Year of Plenty
            endTurn = false;  // Do not end the turn after Year of Plenty
            } else {
                endTurn = true;  // End the turn after using Monopoly or Road Building
            }

            return CardUseError::Success;
        }

        // Second, handle non-promotion cards
        if (developmentCards[cardType] < 1) 
        {
            cout << "Failed to use " << devCardTypeToString(cardType) << ". Reason: Insufficient cards." << endl;
            return CardUseError::InsufficientCards;
        }

        // Handle specific card types
        switch (cardType) 
        {
            // case DevCardType::KNIGHT:
            //     useKnightCard();
            //     break;
            case DevCardType::VICTORY_POINT:
                addPoints(1); // Directly add a victory point
                cout << name << " gained a victory point from a Victory Point Card." << endl;
                break;
            default:
                cout << "Invalid card type or other error." << endl;
                return CardUseError::InvalidCardType;
        }

        endTurn = true; // Turn ends after using any development card successfully
        developmentCards[cardType]--;  // Decrement the card after use
        cout << "Used " << devCardTypeToString(cardType) << " card. Remaining count: " << developmentCards[cardType] << endl;

        return CardUseError::Success;
    }


    void Player::additionalActions(Board& board) {
        cout << "You have activated the 'Year of Plenty' card and may take two additional actions with your new resources." << endl;
        int actionCount = 2;  // Allowing two actions as per the card's benefit

        while (actionCount > 0) {
            cout << "You have " << actionCount << " actions remaining." << endl;
            cout << "Choose an action:\n";
            cout << "1. Build a road\n";
            cout << "2. Build a settlement\n";
            cout << "3. Upgrade to a city\n";
            cout << "4. Pass (skip action)\n";
            cout << "Enter your choice: ";
            int choice;
            cin >> choice;

            int intersectionID1, intersectionID2;

            switch (choice) {
                case 1:
                    cout << "Enter the intersection IDs to place a road (e.g., 4 5): ";
                    cin >> intersectionID1 >> intersectionID2;
                    try {
                        Edge edge(Intersection::getIntersection(intersectionID1), Intersection::getIntersection(intersectionID2));
                        if (canBuild("road") && board.canPlaceRoad(edge, this->id)) {
                            buildRoad(edge, board);
                            cout << "Road built successfully." << endl;
                            actionCount--;
                        } else {
                            cout << "Failed to build road. Check if the road is valid or if you have enough resources." << endl;
                        }
                    } catch (const std::exception& e) {
                        cout << "Invalid intersection IDs provided. Error: " << e.what() << endl;
                    }
                    break;

                case 2:
                    cout << "Enter the intersection ID to place a settlement: ";
                    cin >> intersectionID1;
                    if (canBuild("settlement") && board.canPlaceSettlement(intersectionID1, this->id)) {
                        buildSettlement(intersectionID1, board);
                        cout << "Settlement built successfully." << endl;
                        actionCount--;
                    } else {
                        cout << "Failed to build settlement. Ensure you have enough resources and a valid location." << endl;
                    }
                    break;
                case 3:
                    cout << "Enter the intersection ID to upgrade to a city: ";
                    cin >> intersectionID1;
                    if (canBuild("city") && board.canUpgradeSettlementToCity(intersectionID1, this->id)) {
                        upgradeToCity(intersectionID1, board);
                        cout << "City upgraded successfully." << endl;
                        actionCount--;
                    } else {
                        cout << "Failed to upgrade to a city. Ensure there is a settlement at the location and you have sufficient resources." << endl;
                    }
                    break;
                case 4:
                    cout << "Skipping action." << endl;
                    actionCount--;
                    break;
                default:
                    cout << "Invalid choice. Please choose a valid action." << endl;
                    break;
            }
        }
        cout << "All actions completed or skipped." << endl;
    }


    // void Player::useKnightCard(Board& board) {
    //     knightCardsUsed++;
    //     cout << name << " uses a Knight card, total used: " << knightCardsUsed << "." << endl;
    //     // Other knight card effects go here

    //     checkLargestArmyAward();
    // }

    void Player::usePromotionCard(PromotionType cardType, vector<Player*>& players, Board& board) {
        switch (cardType) {
            case PromotionType::MONOPOLY:
                useMonopoly(players);
                break;
            case PromotionType::ROAD_BUILDING:
                useRoadBuilding(board);
                break;
            case PromotionType::YEAR_OF_PLENTY:
                useYearOfPlenty();
                break;
            default:
                cout << "Invalid promotion card selected." << endl;
                break;
        }
    }


  
    void Player::useMonopoly(vector<Player*>& players) 
    {
        if (promotionCards[PromotionType::MONOPOLY] < 1) 
        {
            cout << "You do not have a Monopoly card to use." << endl;
            return;
        }

        cout << "Select the resource type to monopolize:\n1. Wood\n2. Brick\n3. Wool\n4. Grain\n5. Ore\nEnter your choice: ";
        int choice;
        cin >> choice;
        ResourceType chosenResource = static_cast<ResourceType>(choice - 1);

        int totalCollected = 0;
        for (auto& player : players) {
            if (player->getId() != this->id) {
                int resourceAmount = player->getResourceCount(chosenResource);
                if (resourceAmount > 0) {
                    resources[chosenResource] += resourceAmount;
                    player->useResources(chosenResource, resourceAmount);
                    cout << player->getName() << " gives " << resourceAmount << " " << resourceTypeToString(chosenResource) << " to " << name << "." << endl;
                    totalCollected += resourceAmount;
                }
            }
        }

        cout << name << " now has " << totalCollected << " more " << resourceTypeToString(chosenResource) << "." << endl;
        promotionCards[PromotionType::MONOPOLY]--; // Use up one Monopoly card
    }


    void Player::useRoadBuilding(Board& board) {
    cout << name << " uses Road Building to place two roads at no resource cost." << endl;

    // Debug: Print existing roads
    cout << "Existing roads for " << name << ":" << endl;
    for (const auto& road : roads) {
        cout << "Road between intersections " << road.getId1() << " and " << road.getId2() << endl;
    }

    // This function should allow the player to build two roads.
    int roadsToBuild = 2;
    while (roadsToBuild > 0) {
        int intersection1, intersection2;
        cout << "Enter the IDs of two intersections to place a road (1-54), separated by a space: ";
        cin >> intersection1 >> intersection2;

        // Check if the intersections are valid
        try {
            const Intersection& inter1 = Intersection::getIntersection(intersection1);
            const Intersection& inter2 = Intersection::getIntersection(intersection2);

            // Check adjacency and ownership conditions
            if (!board.areIntersectionsAdjacent(intersection1, intersection2) || 
                (!settlements.count(intersection1) && !settlements.count(intersection2) && !isRoadContinuation(intersection1, intersection2))) {
                cout << "Invalid road placement: intersections are not adjacent or not connected to your settlements or existing roads." << endl;
                continue;
            }

            // Create the edge
            Edge newRoad(inter1, inter2);

            // Check if road can be placed
            if (!board.canPlaceRoad(newRoad, this->id)) {
                cout << "Road cannot be placed between intersections " << intersection1 << " and " << intersection2 
                    << ". It may already be occupied." << endl;
                continue;
            }

            // Place the road
            board.placeRoad(newRoad, this->id);
            roads.insert(newRoad);  // Record the placement of the road
            cout << "Road successfully built between intersections " << intersection1 << " and " << intersection2 << "." << endl;
            roadsToBuild--;

        } catch (const std::out_of_range& e) {
            cout << "Invalid intersection IDs. Please choose valid intersection numbers from 1 to 54." << endl;
        }
    }
}

    bool Player::isRoadContinuation(int id1, int id2) {
        for (const auto& road : roads) {
            if (road.involvesIntersection(id1) || road.involvesIntersection(id2)) {
                return true;
            }
        }
        return false;
    }



    void Player::useYearOfPlenty() {
        cout << name << " is using a 'Year of Plenty' card." << endl;

        // Player selects the first resource
        ResourceType res1 = chooseResource("Choose the first resource to receive:");
        addResource(res1, 1);  // Add one resource card of the selected type

        // Player selects the second resource
        ResourceType res2 = chooseResource("Choose the second resource to receive:");
        addResource(res2, 1);  // Add one resource card of the selected type

        cout << name << " received " << resourceTypeToString(res1) << " and " << resourceTypeToString(res2) << " from the bank." << endl;
        printResources(); // Optionally print updated resources
    }

    ResourceType Player::chooseResource(const string& prompt) {
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



    

    void Player::discardResources(int toDiscard) {
        cout << name << " has more than 7 resources and must discard " << toDiscard << " of them." << endl;
        int totalDiscarded = 0;
        while (totalDiscarded < toDiscard) {
            cout << "You need to discard " << toDiscard - totalDiscarded << " more resources." << endl;
            cout << "Current resources: WOOD: " << resources[WOOD] << ", BRICK: " << resources[BRICK]
                << ", WOOL: " << resources[WOOL] << ", GRAIN: " << resources[GRAIN] << ", ORE: " << resources[ORE] << endl;
            cout << "Type the number of each resource to discard separated by space (WOOD BRICK WOOL GRAIN ORE): ";
            int wood, brick, wool, grain, ore;
            cin >> wood >> brick >> wool >> grain >> ore;

            // Validate the input
            if (wood <= resources[WOOD] && brick <= resources[BRICK] &&
                wool <= resources[WOOL] && grain <= resources[GRAIN] &&
                ore <= resources[ORE] && (wood + brick + wool + grain + ore) == (toDiscard - totalDiscarded)) {
                resources[WOOD] -= wood;
                resources[BRICK] -= brick;
                resources[WOOL] -= wool;
                resources[GRAIN] -= grain;
                resources[ORE] -= ore;
                totalDiscarded += wood + brick + wool + grain + ore;
            } else {
                cout << "Invalid input. Please ensure the numbers are correct and total the amount you need to discard." << endl;
            }
        }
        cout << "Discarding complete. " << name << " now has:" << endl;
        printResources();

    }



    ResourceType Player::stringToResourceType(const string& input) {
        if (input == "Wood") return ResourceType::WOOD;
        if (input == "Brick") return ResourceType::BRICK;
        if (input == "Wool") return ResourceType::WOOL;
        if (input == "Grain") return ResourceType::GRAIN;
        if (input == "Ore") return ResourceType::ORE;
        throw std::invalid_argument("Invalid resource type");
    }

    int Player::countTotalResources() const {
        int sum = 0;
        for (const auto& resource : resources) {
            sum += resource.second;
        }
        return sum;
    }
    
    void Player::trade(std::vector<Player*>& allPlayers) {
        std::cout << "Do you want to trade resources or cards? (Enter '1' or '2'):\n";
        std::string tradeType;
        std::cin >> tradeType;

        if (tradeType == "1") {
            // Existing resource trading logic
            std::cout << "Choose a player to trade with:" << std::endl;
            for (size_t i = 0; i < allPlayers.size(); ++i) {
                if (allPlayers[i]->getId() != this->getId()) {
                    std::cout << i + 1 << ". " << allPlayers[i]->getName() << std::endl;
                }
            }

            size_t playerIndex;
            std::cin >> playerIndex;
            playerIndex--;  // Adjust for zero-indexing

            if (playerIndex < 0 || playerIndex >= allPlayers.size() || allPlayers[playerIndex]->getId() == this->getId()) {
                std::cout << "Invalid player selection." << std::endl;
                return;
            }

            Player* recipient = allPlayers[playerIndex];
            std::map<ResourceType, int> offerResources, requestResources;

            std::cout << "Your resources before trade:" << std::endl;
            this->printResources();
            std::cout << recipient->getName() << "'s resources before trade:" << std::endl;
            recipient->printResources();

            collectTradeDetails("Enter the number of each resource you want to offer:", offerResources);
            collectTradeDetails("Enter the number of each resource you want in return:", requestResources);

            if (!hasSufficientResources(offerResources)) {
                std::cout << "You do not have enough resources to make this offer." << std::endl;
                return;
            }

            if (!recipient->hasSufficientResources(requestResources)) {
                std::cout << "The requested player does not have enough resources to fulfill your request." << std::endl;
                return;
            }

            std::cout << "Do you accept this trade? (yes/no): ";
            std::string response;
            std::cin >> response;
            if (response == "yes") {
                executeTrade(*this, *recipient, offerResources, requestResources);
                std::cout << "Trade completed successfully." << std::endl;
            } else {
                std::cout << "Trade rejected." << std::endl;
            }
        } else if (tradeType == "2") {
            tradeCards(allPlayers);  // Call the new function for card trading
        } else {
            std::cout << "Invalid input. Please enter 'resources' or 'cards'." << std::endl;
        }
    }


    void Player::collectTradeDetails(const std::string& prompt, std::map<ResourceType, int>& details) {
        std::cout << prompt << std::endl;
        for (auto& resource : {ResourceType::WOOD, ResourceType::BRICK, ResourceType::WOOL, ResourceType::GRAIN, ResourceType::ORE}) {
            std::cout << resourceTypeToString(resource) << ": ";
            int count;
            std::cin >> count;
            details[resource] = count;
        }
    }

    void Player::printTradeDetails(const std::map<ResourceType, int>& offer, const std::map<ResourceType, int>& request) {
        std::cout << "Offers:" << std::endl;
        for (const auto& item : offer) {
            std::cout << "  " << resourceTypeToString(item.first) << ": " << item.second << std::endl;
        }
        std::cout << "Requests:" << std::endl;
        for (const auto& item : request) {
            std::cout << "  " << resourceTypeToString(item.first) << ": " << item.second << std::endl;
        }
    }

    void Player::executeTrade(Player& offerer, Player& recipient, const std::map<ResourceType, int>& offerResources, const std::map<ResourceType, int>& requestResources) {
        for (const auto& item : offerResources) {
            offerer.resources[item.first] -= item.second;
            recipient.resources[item.first] += item.second;
        }
        for (const auto& item : requestResources) {
            recipient.resources[item.first] -= item.second;
            offerer.resources[item.first] += item.second;
        }
    }

    bool Player::hasSufficientResources(const std::map<ResourceType, int>& resourcesNeeded) {
        for (const auto& item : resourcesNeeded) {
            if (resources[item.first] < item.second) {
                return false;
            }
        }
        return true;
    }

    void Player::tradeCards(std::vector<Player*>& allPlayers) {
        std::cout << "Choose a player to trade cards with:" << std::endl;
        for (size_t i = 0; i < allPlayers.size(); ++i) {
            if (allPlayers[i]->getId() != this->getId()) {
                std::cout << i + 1 << ". " << allPlayers[i]->getName() << std::endl;
            }
        }

        size_t playerIndex;
        std::cin >> playerIndex;
        playerIndex--;  // Adjust for zero-indexing

        if (playerIndex < 0 || playerIndex >= allPlayers.size() || allPlayers[playerIndex]->getId() == this->getId()) {
            std::cout << "Invalid player selection." << std::endl;
            return;
        }

        Player* recipient = allPlayers[playerIndex];
        // Assume a structure to hold card offers and requests
        std::map<DevCardType, int> offerCards, requestCards;

        std::cout << "Your cards before trade:" << std::endl;
        this->printCardCounts();
        std::cout << recipient->getName() << "'s cards before trade:" << std::endl;
        recipient->printCardCounts();

        collectCardTradeDetails("Enter the number of each card type you want to offer:", offerCards);
        collectCardTradeDetails("Enter the number of each card type you want in return:", requestCards);

        if (!hasSufficientCards(offerCards)) {
            std::cout << "You do not have enough cards to make this offer." << std::endl;
            return;
        }

        if (!recipient->hasSufficientCards(requestCards)) {
            std::cout << "The requested player does not have enough cards to fulfill your request." << std::endl;
            return;
        }

        std::cout << "Do you accept this trade? (yes/no): ";
        std::string response;
        std::cin >> response;
        if (response == "yes") {
            executeCardTrade(*this, *recipient, offerCards, requestCards, allPlayers);
            std::cout << "Card trade completed successfully." << std::endl;
        } else {
            std::cout << "Card trade rejected." << std::endl;
        }
    }

    void Player::collectCardTradeDetails(const string& prompt, map<DevCardType, int>& cardDetails) {
        cout << prompt << endl;
        cout << "1. Knight Cards\n2. Victory Point Cards\n3. Monopoly Cards\n4. Road Building Cards\n5. Year of Plenty Cards\nEnter the number of each card type you want in return:" << endl;
        int knights, victoryPoints, monopolies, roadBuildings, yearsOfPlenty;
        cout << "Knight Cards: "; cin >> knights;
        cout << "Victory Point Cards: "; cin >> victoryPoints;
        cout << "Monopoly Cards: "; cin >> monopolies;
        cout << "Road Building Cards: "; cin >> roadBuildings;
        cout << "Year of Plenty Cards: "; cin >> yearsOfPlenty;

        cardDetails[DevCardType::KNIGHT] = knights;
        cardDetails[DevCardType::VICTORY_POINT] = victoryPoints;
        cardDetails[DevCardType::PROMOTION] = monopolies + roadBuildings + yearsOfPlenty;  // Aggregate promotion cards for simplicity unless differentiated in map
    }

    bool Player::hasSufficientCards(const map<DevCardType, int>& cardsNeeded) {
        for (const auto& [type, required] : cardsNeeded) {
            if (developmentCards[type] < required)
                return false;
        }
        return true;
    }

    // void Player::executeCardTrade(Player& offerer, Player& recipient, const map<DevCardType, int>& offerCards, const map<DevCardType, int>& requestCards, vector<Player*>& allPlayers) {
    //     for (const auto& [type, quantity] : offerCards) {
    //         offerer.developmentCards[type] -= quantity;
    //         recipient.developmentCards[type] += quantity;
    //     }
    //     for (const auto& [type, quantity] : requestCards) {
    //         recipient.developmentCards[type] -= quantity;
    //         offerer.developmentCards[type] += quantity;
    //     }

    //     // After trading cards, reevaluate the largest army status if knight cards were traded
    //     if (offerCards.count(DevCardType::KNIGHT) > 0 || requestCards.count(DevCardType::KNIGHT) > 0) {
    //         reevaluateLargestArmy(allPlayers); // Assuming you have access to `allPlayers` here
    //     }

    //     cout << "Trade executed successfully." << endl;
    // }

    void Player::executeCardTrade(Player& offerer, Player& recipient, const map<DevCardType, int>& offerCards, const map<DevCardType, int>& requestCards, vector<Player*>& allPlayers) {
        bool knightCardTraded = false;

        for (const auto& [type, quantity] : offerCards) {
            offerer.developmentCards[type] -= quantity;
            recipient.developmentCards[type] += quantity;
            if (type == DevCardType::KNIGHT) {
                knightCardTraded = true;
            }
        }

        for (const auto& [type, quantity] : requestCards) {
            recipient.developmentCards[type] -= quantity;
            offerer.developmentCards[type] += quantity;
            if (type == DevCardType::KNIGHT) {
                knightCardTraded = true;
            }
        }

        cout << "Trade executed successfully." << endl;

        // If knight cards were involved in the trade, reevaluate the largest army
        if (knightCardTraded) {
            reevaluateLargestArmy(allPlayers); // Ensure this function is implemented to adjust the largest army correctly
        }
    }



    const map<DevCardType, int>& Player::getDevelopmentCards() const {
        return developmentCards;
    }

    int Player::getResourceCount(ResourceType type) const {
        auto it = resources.find(type);
        if (it != resources.end()) {
            return it->second;
        }
        return 0;
    }

    int Player::getPoints() const 
    {
        return this->points;
    }

    void Player::endTurn() {
        cout << name << " has ended their turn." << endl;
        // Any additional cleanup or end-of-turn actions can be added here
    }

    int Player::rollDice() {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<> dis(1, 6);
        return dis(gen);
    }

    string Player::getName() const 
    { 
        return name; 
    }
    
    int Player::getId() const 
    { 
        return id; 
    }

    void Player::printPoints() const 
    {
        cout << name << " has " << points << " points." << endl;
    }

    string Player::printPlayer() const {
        stringstream ss;
        ss << "Player " << name << " (Id " << id << "):\n";
        ss << "Points: " << points << "\n";
        // Print if the player has the Largest Army
        if (largestArmyHolder == this) {
            ss << "Largest Army Card: V\n";
        }
        // Print settlements
        ss << "Settlements at: ";
        for (int settlement : settlements) {
            ss << settlement << " ";
        }   
        // Print cities
        ss << "\nCities at: ";
        for (int city : cities) {
            ss << city << " ";
        }
        // Print roads
        ss << "\nRoads on: ";
        for (const Edge& road : roads) {
            ss << "(" << road.getId1() << ", " << road.getId2() << ") ";
        }
        ss << "\n";

        // Print resources
        ss << "Resources:\n";
        string resources_str;
        for (const auto& [resourceType, amount] : resources) {
            resources_str += resourceTypeToString(resourceType) + " [" + std::to_string(amount) + "], ";
        }
        if (!resources_str.empty()) {
            resources_str.pop_back();  // Remove last space
            resources_str.pop_back();  // Remove last comma
        }
        ss << resources_str;

        // Development Cards
        ss << "\nDevelopment Cards:\n";
        try {
            ss << "KNIGHT [" << developmentCards.at(DevCardType::KNIGHT) << "]\n";
            ss << "VICTORY_POINT [" << developmentCards.at(DevCardType::VICTORY_POINT) << "]\n";
            // Promotion Cards
            ss << "PROMOTION: ";
            ss << "MONOPOLY [" << promotionCards.at(PromotionType::MONOPOLY) << "], ";
            ss << "ROAD_BUILDING [" << promotionCards.at(PromotionType::ROAD_BUILDING) << "], ";
            ss << "YEAR_OF_PLENTY [" << promotionCards.at(PromotionType::YEAR_OF_PLENTY) << "]";
        } catch (const std::out_of_range& e) {
            // Handle the case where a key does not exist, although in your case, this should not happen
            ss << "Error accessing card data.";
        }
        ss << "\n";
        return ss.str();
    }

    void Player::printCardCounts() const {
        cout << "Card counts for " << name << " (Id " << id << "):" << endl;
        cout << "  Knight Cards: " << developmentCards.at(DevCardType::KNIGHT) << endl;
        cout << "  Victory Point Cards: " << developmentCards.at(DevCardType::VICTORY_POINT) << endl;
        cout << "  Promotion Cards:" << endl;
        cout << "    Monopoly: " << promotionCards.at(PromotionType::MONOPOLY) << endl;
        cout << "    Road Building: " << promotionCards.at(PromotionType::ROAD_BUILDING) << endl;
        cout << "    Year of Plenty: " << promotionCards.at(PromotionType::YEAR_OF_PLENTY) << endl;
    }


    // modified functions for tests////

    void Player::buildSettlementForTesting(int intersectionID, Board& board) {
        cout << "Trying to build settlement at intersection (Testing) " << intersectionID << endl;
        cout << "Resource check (can build 'settlement'): " << canBuild("settlement") << endl;

        // This function will ignore the road connection requirement
        if (canBuild("settlement")) {
            // Deduct resources
            resources[ResourceType::BRICK] -= 1;
            resources[ResourceType::WOOD] -= 1;
            resources[ResourceType::WOOL] -= 1;
            resources[ResourceType::GRAIN] -= 1;
            
            // Add settlement
            settlements.insert(intersectionID);
            board.placeSettlement(intersectionID, this->id);
            addPoints(1);  // Assuming 1 point per settlement

            cout << "Settlement built successfully at intersection (Testing) " << intersectionID << endl;
        } else {
            cout << "Failed to build settlement at intersection (Testing) " << intersectionID << endl;
        }
    }


    
}