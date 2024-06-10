// Email: origoldbsc@gmail.com

#include "player.hpp"
#include <iostream>
#include <sstream>

using namespace std;
namespace ariel {

    int Player::nextID = 1;                             // Initialize the static member to 1
    Player* Player::largestArmyHolder = nullptr;        // Initialize the static member to nulll pointer

    /**
     * @brief Constructs a new player with the specified name.
     * Initializes the player's ID, resources, development cards, and promotional cards.
     * All resources and cards are initialized to zero.
     * @param name The name of the player, which is used to identify the player in the game.
     */
    Player::Player(const string& name) : name(name), id(nextID++), resources(), developmentCards(), points(0) { 
        
        // Initialize all resource types to zero
        for (auto& resource : {ResourceType::WOOD, ResourceType::BRICK, ResourceType::WOOL, ResourceType::GRAIN, ResourceType::ORE}) 
        {
            resources[resource] = 0;
        }

        // Initialize promotion card types to zero
        promotionCards[PromotionType::MONOPOLY] = 0;
        promotionCards[PromotionType::ROAD_BUILDING] = 0;
        promotionCards[PromotionType::YEAR_OF_PLENTY] = 0;

        // Initialize other development card types to zero
        developmentCards[DevCardType::KNIGHT] = 0;
        developmentCards[DevCardType::VICTORY_POINT] = 0;
    }


    /**
     * @brief A map that defining the resource costs for building different structures in the game.
     * Each type of structure (road, settlement, city) requires a combination of specific resources.
     * This map is utilized to check and deduct the required resources from a player's inventory when they attempt to build.
     *
     * - Roads require 1 brick and 1 wood.
     * - Settlements require 1 of each: brick, wood, wool, and grain.
     * - Cities require 3 ore and 2 grain.
     */
    const map<string, map<ResourceType, int>> Player::buildingCosts = 
    {
        {"road", {{ResourceType::BRICK, 1}, {ResourceType::WOOD, 1}}},
        {"settlement", {{ResourceType::BRICK, 1}, {ResourceType::WOOD, 1}, {ResourceType::WOOL, 1}, {ResourceType::GRAIN, 1}}},
        {"city", {{ResourceType::ORE, 3}, {ResourceType::GRAIN, 2}}}
    };


    /**
     *  @brief A map that defining the resource costs for purchasing development cards.
     * Each development card requires a combination of ore, wool, and grain.
     *
     * - Each development card requires 1 ore, 1 wool, and 1 grain.
     */
    const map<ResourceType, int> Player::devCardCosts = 
    {
        {ResourceType::ORE, 1},
        {ResourceType::WOOL, 1},
        {ResourceType::GRAIN, 1}
    };

    
    /**
     * @brief Determines whether the player has sufficient resources to build a specified type of structure.
     * This function checks if the player's available resources meet or exceed the required amounts 
     * for the structure as defined in the `buildingCosts` map.
     * @param structureType A string representing the type of structure to be built. This can be "road", "settlement", or "city".
     * @return True if the player has all necessary resources to build the specified structure; False otherwise.
     *
     * Example Usage:
     * - canBuild("road") checks if the player has at least 1 brick and 1 wood.
     * - canBuild("settlement") checks if the player has at least 1 brick, 1 wood, 1 wool, and 1 grain.
     * - canBuild("city") checks if the player has at least 3 ore and 2 grain.
     */    
    bool Player::canBuild(const string& structureType) 
    {
        const auto& cost = buildingCosts.at(structureType);         // Return the cost of building the specified structure from map
        
        // Loop through each resource and quantity pair needed to build the specified structure
        for (const auto& resource : cost) 
        {
            // Check if the player's current resource counts for each required resource type is less than the required amount.
            // The `first` of the pair represents the ResourceType (enum), and the `second` of the pair represents the required quantity (int).
            if (resources[resource.first] < resource.second)
                return false;
            }
        return true;
    }

    /**
     * @brief Checks if a new road between two intersection IDs can be considered a continuation of existing roads.
     * This method is used to ensure that new roads connect to the existing road network of the player.
     * It checks all existing roads of the player to see if any involve either of the two intersection IDs.
     * @param id1 The ID of the first intersection.
     * @param id2 The ID of the second intersection.
     * @return true if the new road connects to existing roads at either intersection; false otherwise.
     */
    bool Player::isRoadContinuation(int id1, int id2) 
    {
        // Check if the current road involves either of the two intersections.
        // The involvesIntersection method (in Edge class) should return true if the road is connected to the intersection ID.
        for (const auto& road : roads) 
        {
            if (road.involvesIntersection(id1) || road.involvesIntersection(id2)) 
            {
                return true;
            }
        }
        return false;       // If no existing roads involve either of the intersection IDs, return false.
    }

    /**
     * @brief Places an initial settlement for this player at a specified intersection on the board.
     * This method is used during the setup phase of the game, where players place their starting settlements.
     * @param intersectionID The ID of the intersection where the settlement is to be placed.
     * @param board A reference to the Board object representing the game board.
     */
    void Player::placeInitialSettlement(int intersectionID, Board& board) 
    {
        board.placeInitialSettlement(intersectionID, this->id);         // "forward" the settlement to the Board class, which handles the game board
        settlements.insert(intersectionID);                             // Update the player's record of settlements
        addPoints(1);                                                   // Increases the player's score by 1
    }


    /**
     * @brief Places an initial road for this player on the specified edge of the board.
     * This method is used during the setup phase of the game, where players place their starting roads.
     * @param edge The edge on which the road is to be placed, represented by an Edge object.
     * @param board A reference to the Board object representing the game board.
     */
    void Player::placeInitialRoad(const Edge& edge, Board& board) 
    {
        board.placeInitialRoad(edge, this->id);         //"forward" the settlement to the Board class, which handles the game board
        this->roads.insert(edge);                       // Update the player's record of roads
    }


    

    /**
     * @brief Attempts to build a settlement at a specified intersection on the board.
     * This method first checks if the player has the necessary resources and if the board conditions
     * allow for a settlement to be placed at the specified intersection. If both conditions are met,
     * it deducts the required resources, updates the game state, and records the new settlement.
     * @param intersectionID The ID of the intersection where the settlement is to be built.
     * @param board A reference to the Board object representing the game board.
     */
    void Player::buildSettlement(int intersectionID, Board& board)
    {
        // Check if the player can build a settlement based on their resources and if the board rules allow placing a settlement at this location.
        if (canBuild("settlement") && board.canPlaceSettlement(intersectionID, this->id)) 
        {
            // Deduct the required resources for building a settlement
            resources[ResourceType::BRICK] -= 1;
            resources[ResourceType::WOOD] -= 1;
            resources[ResourceType::WOOL] -= 1;
            resources[ResourceType::GRAIN] -= 1;
            
            settlements.insert(intersectionID);                     // Record the new settlement
            board.placeSettlement(intersectionID, this->id);        // Place the settlement on the board
            addPoints(1);                                           // Increment the player's points by 1
        }
    }


    /**
     * @brief Attempts to build a road on a specified edge of the board.
     * This method checks if the player has the necessary resources (brick and wood) and if the board conditions
     * allow for a road to be placed on the specified edge. If both conditions are satisfied, it deducts the required
     * resources, records the new road, and updates the game state on the board.
     * @param edge The Edge object representing the edge where the road is to be built.
     * @param board A reference to the Board object representing the game board.
     */
    void Player::buildRoad(const Edge& edge, Board& board) 
    {
        // Check if the player can build a road based on their resources and if the board rules allow placing the road
        if (canBuild("road") && board.canPlaceRoad(edge, this->id)) 
        {
            // Deduct the required resources for building a road.
            resources[ResourceType::BRICK] -= 1;
            resources[ResourceType::WOOD] -= 1;
            
            roads.insert(edge);                 // Record the new road
            board.placeRoad(edge, this->id);    // place the road on the board
        }
    }


    /**
     * @brief Tries to upgrade a settlement to a city at a specified intersection on the board.
     * This method checks if a settlement exists at the given intersection and if the player has the necessary
     * resources to upgrade it to a city. If both conditions are met, the settlement is upgraded to a city, resources
     * are deducted, and the game state on the board is updated.
     * @param intersectionID The ID of the intersection where the settlement is to be upgraded to a city.
     * @param board A reference to the Board object representing the game board.
     */
    void Player::upgradeToCity(int intersectionID, Board& board) 
    {
        // Check if there is a settlement at the specified intersection and if the player can build a city
        if (settlements.count(intersectionID) && canBuild("city")) 
        {
            // Check if there are enough resources for the player
            if (useResources(ResourceType::ORE, 3) && useResources(ResourceType::GRAIN, 2)) 
            {
                settlements.erase(intersectionID);          // Remove from settlement
                cities.insert(intersectionID);              // Add the upgraded city to the set of cities
            
                board.upgradeSettlementToCity(intersectionID, this->id);    // place the city on the board
                
                addPoints(1);  // Adjust points (city gives 2 points, removing a settlement subtracts the 1 point)

                // cout << "Upgraded settlement at intersection " << intersectionID << " to a city." << endl;
            } 
            else 
            {
                cout << "Not enough resources to upgrade to a city." << endl;
            }
        } 
        else 
        {
            cout << "No settlement to upgrade at intersection " << intersectionID << endl;
        }
    }


    /**
     * @brief Adds a specified quantity of a resource to the player.
     * This method updates the player's resource count for a specific type. It is called when the player gains
     * resources while using Year Of Plenty card.
     * @param type The type of resource to be added, as defined by the ResourceType enumeration.
     * @param quantity The amount of the resource to be added.
     */
    void Player::addResource(ResourceType type, int quantity) 
    {
        resources[type] += quantity;
        
        // cout << "Added " << quantity << " " << resourceTypeToString(type) << " to Player " << id         // printings for debuging
        // << ", total now: " << resources[type] << endl;
        // printResources(); // Calling to print all resources after updating

    }

    /**
     * @brief Deducts a quantity of a resource from the player if sufficient resources are available.
     * This method checks if the player has at least the requested quantity of the specified resource type.
     * If the player has enough resources, it deducts the specified quantity and returns true to indicate
     * the operation was successful. If the player does not have enough resources, the method returns false.
     * @param type The type of resource to deduct, as defined by the ResourceType enumeration.
     * @param quantity The amount of the resource to be deducted.
     * @return true if the resources were successfully deducted; false otherwise.
     */
    bool Player::useResources(ResourceType type, int quantity) 
    {
        if (resources[type] >= quantity) 
        {
            resources[type] -= quantity;
            return true;
        }
        return false;
    }


    /**
     * @brief Returns the current count of a specified resource in the player's inventory.
     * This method searches the player's resources map for the given type of resource. If the resource
     * type is found, it returns the associated count. If the resource type is not found (which might
     * happen if no transactions involving that resource have occurred yet), it returns 0, indicating
     * that the player has none of that resource.
     * @param type The type of resource to query, as defined by the ResourceType enumeration.
     * @return The count of the specified resource type; returns 0 if the resource is not found.
     */
    int Player::getResourceCount(ResourceType type) const 
    {
        // Use the find method (iterator) to locate the resource type in the resources map
        auto it = resources.find(type);
        
        // If the iterator does not equal the end iterator, it means the resource type was found in the map
        if (it != resources.end()) 
        {
            return it->second;          // Return the quantity of the resource found
        }
        return 0;
    }


    /**
     * @brief Prints the current resources of the player to the console.
     * This method iterates through the player's resource map and prints out each resource type
     * along with its corresponding count. It provides a visual summary of the resources,
     * which is helpful for debugging and game status updates.
     */
    void Player::printResources() const 
    {
        cout << name << "'s resources: ";
        
        // Iterate through each pair in the resources map
        for (const auto& [resourceType, amount] : resources) 
        {
            cout << resourceTypeToString(resourceType) << ": " << amount << ", ";
        }
        cout << endl;
    }


   /**
     * @brief Tries to buy a development card and handles all necessary checks and adjustments.
     * @param cardType The type of development card to purchase.
     * @param allPlayers A list of all players in the game for necessary checks.
     * @return The result of the card purchase attempt.
     */
    CardPurchaseError Player::buyDevelopmentCard(DevCardType cardType, vector<Player*>& allPlayers)
    {
        // First, check if the player has enough resources for buying the card
        if (!hasEnoughResourcesForCard()) 
        {
            return CardPurchaseError::InsufficientResources;
        }

        // Second, check if cards left in catch
        if (DevelopmentCard::getCardQuantity(cardType) <= 0) 
        {
            return CardPurchaseError::CardUnavailable;
        }
        
        // Deduct resources
        resources[ResourceType::ORE] -= 1;
        resources[ResourceType::WOOL] -= 1;
        resources[ResourceType::GRAIN] -= 1;

        return handleCardPurchase(cardType, allPlayers);
    }


    /**
     * @brief Checks if the player has enough resources to buy the development card.
     * @return True if sufficient resources are available, otherwise false.
     */
    bool Player::hasEnoughResourcesForCard() const 
    {
        try {
            // Check if there are enough of each resource type 
            return resources.at(ResourceType::ORE) >= 1 && resources.at(ResourceType::WOOL) >= 1 && resources.at(ResourceType::GRAIN) >= 1;
        } 
        catch (const out_of_range& e) 
        {
            return false;
        }
    }

    /**
     * @brief Handles the specific logic related to purchasing different types of development cards.
     * @param cardType The type of development card being purchased.
     * @param allPlayers A reference to all players in the game, used for certain card effects.
     * @return The result of the card purchase attempt.
     */
    CardPurchaseError Player::handleCardPurchase(DevCardType cardType, vector<Player*>& allPlayers) 
    {
        if (cardType == DevCardType::KNIGHT) 
        {
            KnightCard::decreaseQuantity();
            developmentCards[DevCardType::KNIGHT]++;
            knightCards++;
            checkForLargestArmy(allPlayers);
        } 
        else if (cardType == DevCardType::VICTORY_POINT) 
        {
            VictoryPointCard::decreaseQuantity();
            developmentCards[DevCardType::VICTORY_POINT]++;
        } 
        else if (cardType == DevCardType::PROMOTION) 
        {
            return handlePromotionCardPurchase();
        } 
        else 
        {
            cout << "Invalid card type." << endl;
            return CardPurchaseError::CardUnavailable;
        }

        // cout << "Card purchased: " << devCardTypeToString(cardType) << ". New count: " << developmentCards[cardType] << endl;    // print for debuging
        return CardPurchaseError::Success;
    }

    /**
     * @brief Manages the process of buying a promotion card.
     * @return The result of the promotion card purchase attempt.
     */
    CardPurchaseError Player::handlePromotionCardPurchase() 
    {
        cout << "Enter 1 for Monopoly, 2 for Road Building, 3 for Year of Plenty: ";
        int choice;
        cin >> choice;

        switch (choice) 
        {
            case 1:
                return buySpecificPromotionCard(MonopolyCard::getQuantity, MonopolyCard::decreaseQuantity, PromotionType::MONOPOLY);
            case 2:
                return buySpecificPromotionCard(RoadBuildingCard::getQuantity, RoadBuildingCard::decreaseQuantity, PromotionType::ROAD_BUILDING);
            case 3:
                return buySpecificPromotionCard(YearOfPlentyCard::getQuantity, YearOfPlentyCard::decreaseQuantity, PromotionType::YEAR_OF_PLENTY);
            default:
                cout << "Invalid choice, no card added." << endl;
                return CardPurchaseError::CardUnavailable;
        }
    }

    /**
     * @brief A helper function to manage the buying of a specific type of promotion card.
     * @param getQuantity A function that returns the current quantity of the card.
     * @param decreaseQuantity A function that decreases the quantity of the card.
     * @param type The type of promotion card.
     * @return The result of the promotion card purchase attempt.
     */
    CardPurchaseError Player::buySpecificPromotionCard(function<int()> getQuantity, function<void()> decreaseQuantity, PromotionType type) 
    {
        if (getQuantity() > 0) 
        {
            decreaseQuantity();
            promotionCards[type]++;
            developmentCards[DevCardType::PROMOTION]++;
            return CardPurchaseError::Success;
        } 
        else 
        {
            return CardPurchaseError::CardUnavailable;
        }
    }



    /**
     * @brief Handles the use of development cards during gameplay, applying effects based on card type.
     * @param cardType Type of development card being used.
     * @param allPlayers List of all players in the game for effects impacting multiple players.
     * @param board Reference to the game board for any modifications required by card effects.
     * @param endTurn Reference to a boolean indicating whether the turn should end after card use.
     * @return Returns an error status indicating the outcome of the card use attempt.
     */
    CardUseError Player::useDevelopmentCard(DevCardType cardType, vector<Player*>& allPlayers, Board& board, bool& endTurn) 
    {
        cout << "Attempting to use card type: " << devCardTypeToString(cardType) << endl;
        endTurn = false;  // Default to not ending the turn

        // Check card type and handle accordingly
        switch (cardType) {
            case DevCardType::PROMOTION:
                return handlePromotionCardUsage(allPlayers, board, endTurn);
            case DevCardType::KNIGHT:
                // Additional handling if it's a Knight card (not in the game's rule)
                break;
            case DevCardType::VICTORY_POINT:
            {
                VictoryPointCard vpCard;
                return vpCard.activateCard(*this, allPlayers, board, endTurn); 
            }           
            default:
                cout << "Invalid card type or other error." << endl;
                return CardUseError::InvalidCardType;
        }

        // General handling for all cards
        developmentCards[cardType]--;
        cout << "Used " << devCardTypeToString(cardType) << " card. Remaining count: " << developmentCards[cardType] << endl;
        return CardUseError::Success;
    }


    /**
     * @brief Handles the use of a promotion card by prompting the player to choose a specific card type and executing its effects.
     * This method guide the player to select a type of promotion card to use, including Monopoly, Road Building,
     * or Year of Plenty. 
     * @param allPlayers Reference to a vector containing pointers to all players in the game. 
     * @param board Reference to the Board object representing the game board. 
     * @param endTurn Reference to a boolean that indicates whether the turn should end after using the card. 
     * @return CardUseError An enum value that indicates the success or failure of the operation. 
     */
    CardUseError Player::handlePromotionCardUsage(vector<Player*>& allPlayers, Board& board, bool& endTurn) {
        cout << "Promotion card type selected. Choose specific promotion card to use:" << endl;
        cout << "1. Monopoly\n2. Road Building\n3. Year of Plenty\nEnter your choice: ";
        int promoChoice;
        cin >> promoChoice;

        switch (promoChoice) 
        {
            case 1:
            {
                MonopolyCard mc;
                return mc.activateCard(*this, allPlayers, board, endTurn);
            }
            case 2:
            {
                RoadBuildingCard rb;
                return rb.activateCard(*this, board, endTurn);
            }
            case 3:
            {
                YearOfPlentyCard yop;
                return yop.activateCard(*this, board, endTurn);
            }
            default:
                cout << "Invalid promotion type selected.\n";
                return CardUseError::InvalidCardType;
        }
    }


    /**
     * @brief Evaluates if this player should hold the Largest Army card or not.
     *
     * This function checks if this player meets the conditions to claim the Largest Army card (holding 3 knights).
     * If another player has more knight cards, or this player no longer meets the criteria, the award may be revoked or reassigned.
     *
     * @param allPlayers A reference to a vector of all players in the game, used to reevaluate who should hold the award if it is revoked from this player.
     */
    void Player::checkForLargestArmy(vector<Player*>& allPlayers) 
    {
        // Check if this player has the Largest Army
        if (knightCards >= 3) 
        {
            if (largestArmyHolder == nullptr || largestArmyHolder->knightCards < knightCards) 
            {
                if (largestArmyHolder != nullptr && largestArmyHolder != this) 
                {
                    largestArmyHolder->points -= 2;         // Subtract points from the previous holder
                    cout << largestArmyHolder->name << " has lost the Largest Army." << endl;
                }
                largestArmyHolder = this;
                points += 2;  // Award new points to this player
                cout << name << " now holds the Largest Army and gains 2 victory points." << endl;
            }
        } 
        else 
        {
            // Check if this player is currently holding the Largest Army but no longer qualifies
            if (largestArmyHolder == this) 
            {
                largestArmyHolder = nullptr;
                points -= 2;
                cout << name << " has lost the Largest Army." << endl;
                // Re-evaluate the largest army holder among all players
                reevaluateLargestArmy(allPlayers);
            }
        }
    }

    /**
     * @brief Reevaluates which player, if any, should hold the Largest Army card.
     * This function iterates over all players to determine which one meets the conditions to hold the Largest Army card.
     * @param allPlayers A vector containing pointers to all players, used to assess each player's qualifications for the award.
     */
    void Player::reevaluateLargestArmy(vector<Player*>& allPlayers) 
    {
        Player* currentHolder = Player::largestArmyHolder;
        int maxKnights = 0;
        Player* newHolder = nullptr;

        // Identify the player with the highest number of knight cards
        for (Player* player : allPlayers) 
        {
            if (player->developmentCards[DevCardType::KNIGHT] > maxKnights)
            {
                maxKnights = player->developmentCards[DevCardType::KNIGHT];
                newHolder = player;
            }
        }

        // Give the Largest Army card to the new holder if conditions are met
        if (newHolder && maxKnights >= 3 && newHolder != currentHolder) 
        {
            if (currentHolder) 
            {
                currentHolder->points -= 2;     // Subtract points from the previous holder
            }
            Player::largestArmyHolder = newHolder;
            newHolder->points += 2;
            cout << newHolder->getName() << " now holds the Largest Army." << endl;
        } 
        else if (!newHolder || maxKnights < 3) 
        {
            Player::largestArmyHolder = nullptr;    // No player qualifies for largest army
            if (currentHolder) 
            {
                currentHolder->points -= 2;         // Subtract the previous holder loses the points
            }
        }
    }


    /**
     * @brief Manages the discarding of resources for a player who has more than the allowed limit in case the the dices showed 7.
     * @param toDiscard The total number of resources that the player must discard.
     */
    void Player::discardResources(int toDiscard) 
    {
        cout << name << " has more than 7 resources and must discard " << toDiscard << " of them." << endl;
        int totalDiscarded = 0;
        while (totalDiscarded < toDiscard) 
        {
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
            } 
            else 
            {
                cout << "Invalid input. Please ensure the numbers are correct and total the amount you need to discard." << endl;
            }
        }
        cout << "Discarding complete. " << name << " now has:" << endl;
        printResources();

    }



    /**
     * @brief Preforms trading between players, either for resources or cards.
     *
     * This function asks the player whether they want to trade resources or cards. For resources and cards trading,
     * it allows the player to select another player to trade with, specify the resources and cards they offer and request,
     * and then preform the trade if both parties agree.
     *
     * @param allPlayers A vector containing pointers to all players in the game, 
     */
    void Player::trade(vector<Player*>& allPlayers) 
    {
    cout << "Do you want to trade resources or cards? (Enter '1' for resources, '2' for cards):\n";
        string tradeType;
        cin >> tradeType;

        if (tradeType == "1") 
        {
            tradeResources(allPlayers);
        } 
        else if (tradeType == "2") 
        {
            tradeCards(allPlayers);  // Existing logic for card trading
        } 
        else 
        {
            cout << "Invalid input. Please enter '1' for resources or '2' for cards." << endl;
        }
    }
    
    /**
     * @brief Handles the resource trading process with another player.
     *
     * @param allPlayers A vector containing pointers to all players.
     */
    void Player::tradeResources(vector<Player*>& allPlayers) 
    {
        Player* recipient = selectTradePartner(allPlayers);
        if (!recipient) return; // Exit if no valid partner is selected

        map<ResourceType, int> offerResources, requestResources;
        if (!prepareTradeDetails(*recipient, offerResources, requestResources)) return;


        // Display trade details for confirmation
        cout << "Trade details are as follows:\n";
        printTradeDetails(offerResources, requestResources);

        cout << "Do you accept this trade? (yes/no): ";
        string response;
        cin >> response;
        if (response == "yes") {
            executeTrade(*this, *recipient, offerResources, requestResources);
            cout << "Trade completed successfully." << endl;
        } else {
            cout << "Trade rejected." << endl;
        }
    }
    

    /**
     * @brief Selects a trading partner from the list of all players.
     *
     * @param allPlayers A vector of all players in the game.
     * @return Player* Pointer to the selected player or nullptr if selection is invalid.
     */
    Player* Player::selectTradePartner(vector<Player*>& allPlayers) 
    {
        cout << "Choose a player to trade with: " << endl;
        for (size_t i = 0; i < allPlayers.size(); ++i) {
            if (allPlayers[i]->getId() != this->getId()) {
                cout << i + 1 << ". " << allPlayers[i]->getName() << endl;
            }
        }

        size_t playerIndex;
        cin >> playerIndex;
        playerIndex--;  // Adjust for zero-indexing
        if (playerIndex < 0 || playerIndex >= allPlayers.size() || allPlayers[playerIndex]->getId() == this->getId()) {
            cout << "Invalid player selection." << endl;
            return nullptr;
        }

        return allPlayers[playerIndex];
    }

    /**
     * @brief Prepares trade details by collecting offers and requests and validating them.
     *
     * @param recipient The trading partner.
     * @param offerResources Reference to map where offer details will be stored.
     * @param requestResources Reference to map where request details will be stored.
     * @return bool True if trade details are valid, false otherwise.
     */
    bool Player::prepareTradeDetails(Player& recipient, map<ResourceType, int>& offerResources, map<ResourceType, int>& requestResources) 
    {
        cout << "Your resources before trade:" << endl;
        this->printResources();
        cout << recipient.getName() << "'s resources before trade:" << endl;
        recipient.printResources();

        collectTradeDetails("Enter the number of each resource you want to offer: ", offerResources);
        collectTradeDetails("Enter the number of each resource you want in return: ", requestResources);

        if (!hasSufficientResources(offerResources)) 
        {
            cout << "You do not have enough resources to make this offer." << endl;
            return false;
        }
        if (!recipient.hasSufficientResources(requestResources)) 
        {
            cout << "The requested player does not have enough resources to fulfill your request." << endl;
            return false;
        }
        return true;
    }


    /**
     * @brief Collects specific trade details from the player based on the provided prompt.
     * @param prompt A string that is displayed to the user, guiding them on what information to input.
     * @param details A map that will store the resource type as the key and the quantity as the value.
     */
    void Player::collectTradeDetails(const string& prompt, map<ResourceType, int>& details) 
    {
        cout << prompt << endl;
        for (auto& resource : {ResourceType::WOOD, ResourceType::BRICK, ResourceType::WOOL, ResourceType::GRAIN, ResourceType::ORE}) 
        {
            cout << resourceTypeToString(resource) << ": ";
            int count;
            cin >> count;
            details[resource] = count;
        }
    }


    /**
     * @brief Displays the details of a trade offer and request.
     * @param offer A map containing the offered resources and their quantities.
     * @param request A map containing the requested resources and their quantities.
     */
    void Player::printTradeDetails(const map<ResourceType, int>& offer, const map<ResourceType, int>& request) 
    {
        cout << "Offers:" << endl;
        for (const auto& item : offer) {
            cout << "  " << resourceTypeToString(item.first) << ": " << item.second << endl;
        }
        cout << "Requests:" << endl;
        for (const auto& item : request) {
            cout << "  " << resourceTypeToString(item.first) << ": " << item.second << endl;
        }
    }


    /**
     * @brief Executes a trade between two players, exchanging specified resources.
     * This function processes the trade by adjusting the resources of both the offerer and recipient based on the trade details provided.     *
     * @param offerer Reference to the player who is offering resources.
     * @param recipient Reference to the player receiving the offer.
     * @param offerResources A map of resources and their quantities that the offerer is providing.
     * @param requestResources A map of resources and their quantities that the recipient is expected to give in return.
     */
    void Player::executeTrade(Player& offerer, Player& recipient, const map<ResourceType, int>& offerResources, const map<ResourceType, int>& requestResources) 
    {
        for (const auto& item : offerResources) 
        {
            offerer.resources[item.first] -= item.second;
            recipient.resources[item.first] += item.second;
        }
        for (const auto& item : requestResources) 
        {
            recipient.resources[item.first] -= item.second;
            offerer.resources[item.first] += item.second;
        }
    }


    /**
     * @brief Checks if the player has enough of each specified resource.
     * @param resourcesNeeded A map specifying the types and quantities of resources needed.
     * @return bool True if the player has all the required resources in the specified quantities, false otherwise.
     */
    bool Player::hasSufficientResources(const map<ResourceType, int>& resourcesNeeded) 
    {
        for (const auto& item : resourcesNeeded) 
        {
            if (resources[item.first] < item.second) 
            {
                return false;
            }
        }
        return true;
    }


    /**
     * @brief Handles the card trading process with another player.
     * This function prompts the player to choose a trade partner and sets up a card trade,
     * @param allPlayers A vector containing pointers to all players.
     */
    void Player::tradeCards(vector<Player*>& allPlayers) 
    {
        Player* recipient = selectTradePartner(allPlayers);
        if (!recipient) return;    // Exit if no valid partner is selected

        map<DevCardType, int> offerCards, requestCards;
        if (!prepareCardTradeDetails(*recipient, offerCards, requestCards)) return;

        cout << "Card trade details are as follows:" << endl;
        printTradeDetails(offerCards, requestCards);  

        cout << "Do you accept this trade? (yes/no): ";
        string response;
        cin >> response;
        if (response == "yes") 
        {
            executeCardTrade(*this, *recipient, offerCards, requestCards, allPlayers);
            cout << "Card trade completed successfully." << endl;
        } 
        else 
        {
            cout << "Card trade rejected." << endl;
        }
    }


    /**
     * @brief Collects and validates the details of the card trade offer and request.
     * @param recipient The trading partner.
     * @param offerCards Reference to map where card offer details will be stored.
     * @param requestCards Reference to map where card request details will be stored.
     * @return bool True if trade details are valid, false otherwise.
     */
    bool Player::prepareCardTradeDetails(Player& recipient, map<DevCardType, int>& offerCards, map<DevCardType, int>& requestCards) 
    {
        cout << "Your cards before trade:" << endl;
        this->printCardCounts();
        cout << recipient.getName() << "'s cards before trade: "<< endl;
        recipient.printCardCounts();

        collectCardTradeDetails("Enter the number of each card type you want to offer: ", offerCards);
        collectCardTradeDetails("Enter the number of each card type you want in return: ", requestCards);

        if (!hasSufficientCards(offerCards)) 
        {
            cout << "You do not have enough cards to make this offer." << endl;
            return false;
        }

        if (!recipient.hasSufficientCards(requestCards)) 
        {
            cout << "The requested player does not have enough cards to fulfill your request." << endl;
            return false;
        }
        return true;
    }


    /**
     * @brief Collects details for card types a player wants to trade.
     *
     * @param prompt The prompt to display for collecting card details.
     * @param cardDetails Reference to a map to store the card details.
     */
    void Player::collectCardTradeDetails(const string& prompt, map<DevCardType, int>& cardDetails) 
    {
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
        cardDetails[DevCardType::PROMOTION] = monopolies + roadBuildings + yearsOfPlenty;  
    }


    /**
     * @brief Checks if the player has enough of each specified cards.
     * @param cardsNeeded A map specifying the types and quantities of cards needed.
     * @return bool True if the player has all the required cards in the specified quantities, false otherwise.
     */
    bool Player::hasSufficientCards(const map<DevCardType, int>& cardsNeeded) 
    {
        for (const auto& [type, required] : cardsNeeded) {
            if (developmentCards[type] < required)
                return false;
        }
        return true;
    }


    /**
     * @brief Displays the details of card offers and requests in a trade.
     * @param offer A map containing the offered cards and their quantities.
     * @param request A map containing the requested cards and their quantities.
     */
    void Player::printTradeDetails(const map<DevCardType, int>& offer, const map<DevCardType, int>& request) 
    {
        cout << "Card Offers:" << endl;
        for (const auto& item : offer)
        {
            cout << "  " << devCardTypeToString(item.first) << ": " << item.second << endl;
        }
        cout << "Card Requests:" << endl;
        for (const auto& item : request) 
        {
            cout << "  " << devCardTypeToString(item.first) << ": " << item.second << endl;
        }
    }


    /**
     * @brief Executes a trade of development cards between two players.
     * This function transfers specified quantities of development cards between the offerer and the recipient based on
     * the trade details provided. It also checks if Knight cards are traded and reevaluates the largest army if necessary.
     * @param offerer The player offering cards.
     * @param recipient The player receiving cards.
     * @param offerCards A map detailing the types and quantities of cards being offered.
     * @param requestCards A map detailing the types and quantities of cards being requested.
     * @param allPlayers A vector of all players.
     */
    void Player::executeCardTrade(Player& offerer, Player& recipient, const map<DevCardType, int>& offerCards, const map<DevCardType, int>& requestCards, vector<Player*>& allPlayers) 
    {
        bool knightCardTraded = false;

        for (const auto& [type, quantity] : offerCards) 
        {
            offerer.developmentCards[type] -= quantity;
            recipient.developmentCards[type] += quantity;
            if (type == DevCardType::KNIGHT) 
            {
                knightCardTraded = true;        // Track if Knight cards are trade
            }
        }

        for (const auto& [type, quantity] : requestCards) 
        {
            recipient.developmentCards[type] -= quantity;
            offerer.developmentCards[type] += quantity;
            if (type == DevCardType::KNIGHT) 
            {
                knightCardTraded = true;
            }
        }

        cout << "Trade executed successfully." << endl;

        // Reevaluate the largest army if Knight cards were traded
        if (knightCardTraded) 
        {
            reevaluateLargestArmy(allPlayers); 
        }
    }


    /**
     * @brief Signals the end of the current player's turn.
     */
    void Player::endTurn() 
    {
        cout << name << " has ended their turn." << endl;
    }


    /**
     * @brief Simulates the rolling of a six-sided dice.
     * @return int A random number between 1 and 6.
     */
    int Player::rollDice() 
    {
        static random_device rd;
        static mt19937 gen(rd());
        static uniform_int_distribution<> dis(1, 6);
        return dis(gen);
    }


    /**
     * @brief Returns the player's current holding of development cards.
     * @return A reference to the map storing the player's development cards.
     */
    const map<DevCardType, int>& Player::getDevelopmentCards() const 
    {
        return developmentCards;
    }

    void Player::setDevelopmentCardCount (DevCardType cardType, int count) 
    {
        developmentCards[cardType] = count;
    }


    /**
     * @brief Returns the current count of a specified promotion card type.
     * @param type The type of promotion card.
     * @return The count of the specified promotion card type.
     */
    int Player::getPromotionCardCount(PromotionType type) const 
    {
                return promotionCards.at(type);
    }

    /**
     * @brief Sets the count of a specified promotion card type.
     * @param type The type of promotion card.
     * @param count The new count for the specified promotion card type.
     */
    void Player::setPromotionCardCount(PromotionType type, int count) 
    {
        promotionCards[type] = count;
    }

    /**
     * @brief Returns the player's current score in terms of victory points.
     * @return int The total number of victory points the player has accumulated.
     */
    int Player::getPoints() const 
    {
        return this->points;
    }


    /**
     * @brief Returns the name of the player.
     * @return string The name of the player.
     */
    string Player::getName() const 
    { 
        return name; 
    }


    /**
     * @brief Returns the unique identifier of the player.
     * @return int The player's unique ID.
     */
    int Player::getId() const 
    { 
        return id; 
    }


    /**
     * @brief Provides access to the set of cities owned by the player.
     * @return const set<int>& A reference to a set containing the IDs of intersections where the player has cities.
     */
    const set<int>& Player::getCities() const 
    { 
        return cities; 
    }


    /**
     * @brief Adds a specified number of victory points to the player's current score.
     * @param pointsToAdd The number of points to add to the player's total score.
     */
    void Player::addPoints(size_t pointsToAdd) 
    {
        points = points + pointsToAdd;
    }

     
    /**
     * @brief Counts the total number of resources a player has.
     * @return int The total count of all resources the player owns.
     */
    int Player::countTotalResources() const 
    {
        int sum = 0;
        for (const auto& resource : resources) 
        {
            sum += resource.second;
        }
        return sum;
    }

    
    /**
     * @brief Provides access to the set of settlements owned by the player.
     * @return A reference to a set containing the IDs of intersections where the player has settlements.
     */
    const set<int>& Player::getSettlements() const 
    { 
        return settlements; 
    }


    /**
     * @brief Provides access to the set of roads built by the player.
     * @return A reference to a set containing the roads the player has built.
     */
    const set<Edge>& Player::getRoads() const 
    { 
        return roads; 
    }
    
    /**
     * @brief Adds a new road between two intersections to the player's roads.
     * @param edge The Edge object representing the new road.
     */
    void Player::addRoad(const Edge& edge) 
    {
        roads.insert(edge);
    }

   /**
     * @brief Generates a string representation of the player's game state.
     * @return string A formatted string representing the player's current state in the game.
     */
    string Player::printPlayer() const {
        stringstream ss;
        ss << "\n";
        ss << "++++++++++++++++++++++++++\n";
        ss << "++     PLAYER CARD      ++\n";
        ss << "++++++++++++++++++++++++++\n";
        ss << name << " (ID: " << id << ")\n";
        ss << "==========================\n";
        ss << "Points: " << points << "\n";
        ss << "--------------------------\n";
        if (largestArmyHolder == this) 
        {
            ss << "Largest Army Card: V\n";
            ss << "--------------------------\n";
        }
        ss << "Settlements at:\n  ";
        for (int settlement : settlements) 
        {
            ss << settlement << " ";
        }
        ss << "\n--------------------------\n";
        ss << "Cities at:\n  ";
        for (int city : cities)
         {
            ss << city << " ";
        }
        ss << "\n--------------------------\n";
        ss << "Roads on:\n  ";
        for (const Edge& road : roads) 
        {
            ss << "(" << road.getId1() << ", " << road.getId2() << ") ";
        }
        ss << "\n==========================\n";
        ss << "Resources:\n";
        string resources_str;
        for (const auto& [resourceType, amount] : resources) 
        {
            resources_str += "  " + resourceTypeToString(resourceType) + ": [" + to_string(amount) + "]\n";
        }
        ss << resources_str;
        ss << "==========================\n";
        ss << "Development Cards:\n";
        try {
            ss << "  Knight: [" << developmentCards.at(DevCardType::KNIGHT) << "]\n";
            ss << "  Victory Point: [" << developmentCards.at(DevCardType::VICTORY_POINT) << "]\n";
            ss << "  Promotion:\n";
            ss << "    Monopoly: [" << promotionCards.at(PromotionType::MONOPOLY) << "]\n";
            ss << "    Road Building: [" << promotionCards.at(PromotionType::ROAD_BUILDING) << "]\n";
            ss << "    Year of Plenty: [" << promotionCards.at(PromotionType::YEAR_OF_PLENTY) << "]\n";
        } 
        catch (const out_of_range& e) 
        {
            ss << "  Error accessing card data.\n";
        }
        ss << "++++++++++++++++++++++++++\n";
        ss << "++++++++++++++++++++++++++\n";

        return ss.str();
    }


    /**
     * @brief Displays the counts of each type of development and promotion card the player possesses.
     */
    void Player::printCardCounts() const 
    {
        cout << "Card counts for " << name << " (Id " << id << "):" << endl;
        cout << "  Knight Cards: " << developmentCards.at(DevCardType::KNIGHT) << endl;
        cout << "  Victory Point Cards: " << developmentCards.at(DevCardType::VICTORY_POINT) << endl;
        cout << "  Promotion Cards:" << endl;
        cout << "    Monopoly: " << promotionCards.at(PromotionType::MONOPOLY) << endl;
        cout << "    Road Building: " << promotionCards.at(PromotionType::ROAD_BUILDING) << endl;
        cout << "    Year of Plenty: " << promotionCards.at(PromotionType::YEAR_OF_PLENTY) << endl;
    }


    /**
     * @brief Displays the player's current victory points..
     */
    void Player::printPoints() const 
    {
        cout << name << " has " << points << " points." << endl;
    }


    
    /**
     * @brief Builds a settlement for testing purposes, bypassing certain game rules.
     * This function attempts to build a settlement at the specified intersection for testing. It bypasses the usual
     * requirement of having a road connection to the settlement location. It deducts the necessary resources and 
     * updates the player's settlements and points if the resources are available.
     * @param intersectionID The ID of the intersection where the settlement is to be built.
     * @param board A reference to the Board object, used to place the settlement on the game board.
     */
    void Player::buildSettlementForTesting(int intersectionID, Board& board) 
    {
        cout << "Trying to build settlement at intersection (Testing) " << intersectionID << endl;
        cout << "Resource check (can build 'settlement'): " << canBuild("settlement") << endl;

        // This function will ignore the road connection requirement
        if (canBuild("settlement")) 
        {
            // Deduct resources
            resources[ResourceType::BRICK] -= 1;
            resources[ResourceType::WOOD] -= 1;
            resources[ResourceType::WOOL] -= 1;
            resources[ResourceType::GRAIN] -= 1;
            
            // Add settlement
            settlements.insert(intersectionID);
            board.placeSettlement(intersectionID, this->id);
            addPoints(1);  

            cout << "Settlement built successfully at intersection (Testing) " << intersectionID << endl;
        } 
        else 
        {
            cout << "Failed to build settlement at intersection (Testing) " << intersectionID << endl;
        }
    }

    /**
     * @brief Method to check if the player has built any roads.
     */
    bool Player::hasBuiltRoad() const 
    {
        return !roads.empty();  // Check if the set of roads is not empty
    }

    /**
     * @brief Method to check if the player has built any settlements.
     */
    bool Player::hasBuiltSettlement() const 
    {
        return !settlements.empty();  // Check if the set of settlements is not empty
    }
}