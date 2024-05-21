#include "board.hpp"
#include <iostream>

using namespace std;
namespace ariel {
    
    void Board::addTile(Tile* tile) 
    {
        this->tiles.push_back(tile);
    }

    bool Board::isTileAvailable(const string& type, int number) const 
    {
        for (const auto& tile : tiles) 
        {
            if (tile->getResourceType() == type && tile->getNumber() == number) 
            {
                return true;
            }
        }
        return false;
    }

    void Board::printBoard() const 
    {
        for (const auto& tile : tiles) 
        {
            cout << "Tile: " << tile->getResourceType() << ", Number: " << tile->getNumber() << endl;
        }
    }
}