#include "board.hpp"
#include <iostream>

using namespace std;
namespace ariel {
    
    Board::Board() 
    { 
        setupTiles(); 
    }

    void Board::setupTiles() 
    {
         // Beginner fixed layout (assuming a 5x5 grid with the middle part filled as per Catan rules)
        // Row 1
        addTile({0, -2}, Tile(ResourceType::ORE, 10));
        addTile({1, -2}, Tile(ResourceType::WOOL, 2));
        addTile({2, -2}, Tile(ResourceType::GRAIN, 9));

        // Row 2
        addTile({-1, -1}, Tile(ResourceType::GRAIN, 12));
        addTile({0, -1}, Tile(ResourceType::BRICK, 6));
        addTile({1, -1}, Tile(ResourceType::BRICK, 4));
        addTile({2, -1}, Tile(ResourceType::WOOD, 10));

        // Row 3
        addTile({-2, 0}, Tile(ResourceType::ORE, 9));
        addTile({-1, 0}, Tile(ResourceType::WOOL, 4));
        addTile({0, 0}, Tile(ResourceType::NONE, 0)); // NONE represented desert
        addTile({1, 0}, Tile(ResourceType::WOOL, 5));
        addTile({2, 0}, Tile(ResourceType::WOOD, 6));

        // Row 4
        addTile({-2, 1}, Tile(ResourceType::WOOD, 11));
        addTile({-1, 1}, Tile(ResourceType::WOOL, 3));
        addTile({0, 1}, Tile(ResourceType::ORE, 8));
        addTile({1, 1}, Tile(ResourceType::GRAIN, 3));

        // Row 5
        addTile({-2, 2}, Tile(ResourceType::GRAIN, 11));
        addTile({-1, 2}, Tile(ResourceType::WOOD, 5));
        addTile({0, 2}, Tile(ResourceType::WOOD, 8));
    }

    const Tile& Board::getTile(const std::pair<int, int>& position) const
    {
        return tiles.at(position);
    }

    void Board::printBoard() const
    {
        for (const auto& tile : tiles)
        {
            std::cout << "Tile at (" << tile.first.first << ", " << tile.first.second
                << ") - Resource: " << static_cast<int>(tile.second.getResourceType())
                << ", Number: " << tile.second.getNumber() << std::endl;
        }
    }

    void Board::addTile(const std::pair<int, int>& position, const Tile& tile)
    {
        tiles[position] = tile;
    }

    bool Board::isTileAvailable(const std::pair<int, int>& position) const
    {
        return tiles.find(position) == tiles.end();
    }


    // void Board::addTile(Tile* tile) 
    // {
    //     this->tiles.push_back(tile);
    // }

    // bool Board::isTileAvailable(const string& type, int number) const 
    // {
    //     for (const auto& tile : tiles) 
    //     {
    //         if (tile->getResourceType() == type && tile->getNumber() == number) 
    //         {
    //             return true;
    //         }
    //     }
    //     return false;
    // }

    // void Board::printBoard() const 
    // {
    //     for (const auto& tile : tiles) 
    //     {
    //         cout << "Tile: " << tile->getResourceType() << ", Number: " << tile->getNumber() << endl;
    //     }
    // }
}