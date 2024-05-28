#include "player.hpp"
#include <iostream>
#include <sstream>
#include "card_type.hpp"

using namespace std;
namespace ariel {

    int Player::nextID = 1;     // Initialize the static member to 1

    Player::Player(const string& name) : name(name), id(nextID++), resources(), developmentCards(), points(0) { // Start with 2 points
        for (auto& resource : {ResourceType::WOOD, ResourceType::BRICK, ResourceType::WOOL, ResourceType::GRAIN, ResourceType::ORE}) {
            resources[resource] = 0;
        }
    }

    const map<string, map<ResourceType, int>> Player::buildingCosts = 
    {
        {"road", {{ResourceType::BRICK, 1}, {ResourceType::WOOD, 1}}},
        {"settlement", {{ResourceType::BRICK, 1}, {ResourceType::WOOD, 1}, {ResourceType::WOOL, 1}, {ResourceType::GRAIN, 1}}},
        {"city", {{ResourceType::ORE, 3}, {ResourceType::GRAIN, 2}}}
    };

    bool Player::canBuild(const string& structureType) {
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


    void Player::placeSettlement(int intersectionID, Board& board) {
        if (board.canPlaceSettlement(intersectionID, this->id)) {
            board.placeSettlement(intersectionID, this->id);
            settlements.insert(intersectionID);  // Record the placement of the settlement
            points += 1;  // Each settlement should give a point
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
    cout << "Added " << quantity << " " << resourceTypeToString(type) << " to Player " << id 
         << ", total now: " << resources[type] << endl;
    printResources(); // Calling to print all resources after updating

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

    void Player::buyDevelopmentCard(DevCardType type) 
    {
        // Implement the logic to buy a development card
    }

    void Player::useDevelopmentCard(DevCardType type) 
    {
        // Implement the logic to use a development card
    }

    void trade(Player& other, const string& give, const string& receive, int giveAmount, int receiveAmount)
    {
        // Implement the logic to trade resources between players
    }

    int Player::getPoints() const 
    {
        return this->points;
    }

    void Player::endTurn() {
        cout << name << " has ended their turn." << endl;
        // Any additional cleanup or end-of-turn actions can be added here
    }

    int Player::rollDice() 
    {
        return rand() % 6 + 1;
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
        ss << "\n";

        // Print roads
        ss << "Roads on: ";
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
        ss << resources_str << "\n";

        // Print development cards
        ss << "Development Cards:\n";
        string dev_cards_str;
        for (const auto& [cardType, count] : developmentCards) {
            dev_cards_str += devCardTypeToString(cardType) + ": " + std::to_string(count) + ", ";
        }
        if (!dev_cards_str.empty()) {
            dev_cards_str.pop_back();  // Remove last space
            dev_cards_str.pop_back();  // Remove last comma
        }
        ss << dev_cards_str << "\n";

        return ss.str();
    }

    
}