#include "board.hpp"
#include <iostream>

using namespace std;
namespace ariel {
    
    Board::Board() 
    { 
        setupTiles(); 
    }

    //              (0,2)   (1,2)   (2,2)
    //         (-1,1)   (0,1)   (1,1)   (2,1)
    //    (-2,0)   (-1,0)   (0,0)   (1,0)   (2,0)
    //         (-2,-1)   (-1,-1)   (0,-1)   (1,-1)
    //             (-2,-2)   (-1,-2)   (0,-2)

    void Board::setupTiles() 
    {
        // Beginner fixed layout (assuming a 5x5 grid with the middle part filled as per Catan rules)
        // Row 1
        addTile({0, -2}, Tile(ResourceType::ORE, 10));
        addTile({1, 2}, Tile(ResourceType::WOOL, 2));
        addTile({2, 2}, Tile(ResourceType::WOOD, 9));

        // Row 2
        addTile({-1, 1}, Tile(ResourceType::GRAIN, 12));
        addTile({0, 1}, Tile(ResourceType::BRICK, 6));
        addTile({1, 1}, Tile(ResourceType::WOOL, 4));
        addTile({2, 1}, Tile(ResourceType::BRICK, 10));

        // Row 3
        addTile({-2, 0}, Tile(ResourceType::GRAIN, 9));
        addTile({-1, 0}, Tile(ResourceType::WOOD, 11));
        addTile({0, 0}, Tile(ResourceType::NONE, 0));        // NONE represented desert
        addTile({1, 0}, Tile(ResourceType::WOOD, 3));
        addTile({2, 0}, Tile(ResourceType::ORE, 8));

        // Row 4
        addTile({-2, -1}, Tile(ResourceType::WOOD, 8));
        addTile({-1, -1}, Tile(ResourceType::ORE, 3));
        addTile({0, -1}, Tile(ResourceType::GRAIN, 4));
        addTile({1, -1}, Tile(ResourceType::WOOL, 5));

        // Row 5
        addTile({-2, 2}, Tile(ResourceType::BRICK, 5));
        addTile({-1, -2}, Tile(ResourceType::GRAIN, 6));
        addTile({0, -2}, Tile(ResourceType::WOOL, 11));
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

    bool Board::canPlaceSettlement(const std::set<Vertex>& vertices) 
    {
        for (const auto& vertex : vertices) {
            // Check if any vertex already has a settlement
            if (settlements.find(vertex) != settlements.end() && settlements[vertex]) 
            {
                return false;
            }
        }
        
        // Additional logic can include checking for minimum distances between settlements
        
        return true;
    }

    void Board::placeSettlement(const std::set<Vertex>& vertices) 
    {
        for (const auto& vertex : vertices) {
            settlements[vertex] = true;  // Place a settlement at each vertex
            cout << "Placing settlement at (" << vertex.x << ", " << vertex.y << ")." << endl;
        }
    }

    bool Board::canPlaceRoad(const Edge& edge) 
    {
        // Example check for can place road
        if (roads.find(edge) != roads.end()) 
        {
            return false;  // Road already exists
        }

        // Check that the road is connected to at least one existing road or settlement
        bool connected = (settlements.find(edge.v1) != settlements.end() && settlements[edge.v1]) ||
                         (settlements.find(edge.v2) != settlements.end() && settlements[edge.v2]) ||
                         (roads.find({edge.v1, edge.v2}) != roads.end());
        
        return connected;
    }

    void Board::placeRoad(const Edge& edge) 
    {
        roads[edge] = true;  // Place a road on the edge
        cout << "Placing road between (" << edge.v1.x << ", " << edge.v1.y << ") and ("
             << edge.v2.x << ", " << edge.v2.y << ")." << endl;
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