#include "player.hpp"
#include <iostream>

using namespace std;
namespace ariel {

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


    void Player::placeSettlement(const std::set<Vertex>& vertices, Board& board) 
    {
        if (board.canPlaceSettlement(vertices)) 
        {
            board.placeSettlement(vertices);
        }
    }

    void Player::placeRoad(const Edge& edge, Board& board) 
    {
        if (board.canPlaceRoad(edge)) 
        {
            board.placeRoad(edge);
        }
    }


    void placeCity(const string& type, int number, Board& board)
    {
        // Implement the logic to build a city
    }

    void Player::addResource(ResourceType type, int quantity) 
    {
        resources[type] += quantity;
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

    void endTurn()
    {
        // Implement
    }

    int Player::rollDice() 
    {
        return rand() % 6 + 1;
    }

    void Player::printPoints() const 
    {
        cout << name << " has " << points << " points." << endl;
    }
}