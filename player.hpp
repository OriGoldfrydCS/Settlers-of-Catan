#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>
#include <map>
#include "board.hpp"
#include "resource.hpp"
#include "developmentCard.hpp"

using namespace std;
namespace ariel {

    class Player
    {
        private:
            string name;
            map<ResourceType, int> resources;
            map<DevCardType, int> developmentCards;
            size_t points;

        public:

            // Constructor initializes the player with a name
            Player(const string& name) : name(name), resources(), developmentCards(), points(0) {}
            
            // Building methods - these could be abstracted further depending on game rules
            void buildRoad();
            void buildSettlement();
            void buildCity();
            // virtual void buildRoad(const std::string& tile, int number, Board& board) = 0;
            // virtual void buildSettlement(const std::string& tile, int number, Board& board) = 0;
            // virtual void buildCity(const std::string& tile, int number, Board& board) = 0;
            

            // Methods to manage resources
            void addResource(ResourceType type, int quantity);
            bool useResources(ResourceType type, int quantity);
            // void addResources(ResourceType type, int quantity);
            // bool useResources(map<ResourceType, int> cost);

            // Methods to manage development cards
            void buyDevelopmentCard(DevCardType type);
            void useDevelopmentCard(DevCardType type);
            // void buyDevelopmentCard(Board& board);
            // void useDevelopmentCard(const DevelopmentCard& card);
            
            // Trading method
            void trade(Player& other, ResourceType giveType, ResourceType receiveType, int giveAmount, int receiveAmount);
            // virtual void trade(Player* other, const string& give, const string& receive, int giveAmount, int receiveAmount) = 0;
                        
            int getPoints() const;
            void printPoints() const;
    };
}

#endif