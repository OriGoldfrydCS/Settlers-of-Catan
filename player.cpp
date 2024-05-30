#include "player.hpp"
#include <iostream>
#include <sstream>
#include "card_type.hpp"

using namespace std;
namespace ariel {

    int Player::nextID = 1;     // Initialize the static member to 1

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
        this->settlements.insert(intersectionID);  // Update the player's record of settlements
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

    void Player::addPoints(int pointsToAdd) {
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
        //     << ", total now: " << resources[type] << endl;
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

    CardPurchaseError Player::buyDevelopmentCard(DevCardType cardType) {
        
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

            // Check if this purchase qualifies the player for the Largest Army
            if (developmentCards[DevCardType::KNIGHT] == 3) 
            {
                // Award the "Largest Army" if the player has just purchased their third Knight card
                points += 2;  // Assume 2 points for the "Largest Army"
                cout << name << " has earned the 'Largest Army' award and gains 2 victory points!" << endl;
            }
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
                    } else {
                        return CardPurchaseError::CardUnavailable;
                    }
                    break;
                case 2:
                    if (RoadBuildingCard::getQuantity() > 0) {
                        RoadBuildingCard::decreaseQuantity();
                        promotionCards[PromotionType::ROAD_BUILDING]++;
                    } else {
                        return CardPurchaseError::CardUnavailable;
                    }
                    break;
                case 3:
                    if (YearOfPlentyCard::getQuantity() > 0) {
                        YearOfPlentyCard::decreaseQuantity();
                        // promotionCards[PromotionType::YEAR_OF_PLENTY]++;
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
            case DevCardType::KNIGHT:
                useKnightCard();
                break;
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




    void Player::useKnightCard() 
    {
        // Implement the logic for using a Knight card
        cout << name << " uses a Knight card." << endl;
        // Further actions like moving the robber could be implemented here
    }

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
        // Allow player to choose two resources to gain from the bank
        ResourceType res1 = chooseResource();
        ResourceType res2 = chooseResource();
        resources[res1] += 1;
        resources[res2] += 1;
        cout << name << " gains " << resourceTypeToString(res1) << " and " << resourceTypeToString(res2) << " from Year of Plenty." << endl;
    }

    ResourceType Player::chooseResource() {
        // Implementation to choose a resource (could be simplified here)
        return ResourceType::WOOD; // Example default
    }
    

    void trade(Player& other, const string& give, const string& receive, int giveAmount, int receiveAmount)
    {
        // Implement the logic to trade resources between players
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

    
}