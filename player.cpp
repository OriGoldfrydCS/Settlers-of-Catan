#include "player.hpp"
#include <iostream>

using namespace std;
namespace ariel {

    void Player::buildRoad() 
    {
        // Implement the logic to build a road
    }

    void Player::buildSettlement() 
    {
        // Implement the logic to build a settlement
    }

    void Player::buildCity() 
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

    void Player::trade(Player& other, ResourceType giveType, ResourceType receiveType, int giveAmount, int receiveAmount) 
    {
        // Implement the logic to trade resources between players
    }

    int Player::getPoints() const 
    {
        return this->points;
    }

    void Player::printPoints() const 
    {
        cout << name << " has " << points << " points." << endl;
    }
}