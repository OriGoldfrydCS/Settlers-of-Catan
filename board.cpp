#include "board.hpp"
#include <iostream>

using namespace std;
namespace ariel {
    
    Board::Board() 
    { 
        setupTiles(); 
        setupIntersections();
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

    void Board::setupIntersections() 
    {
        // Three-vertex intersections: central-upper part of the board
        intersections[4] = Intersection({Vertex(-1, 1), Vertex(0, 1), Vertex(0, 2)});
        intersections[5] = Intersection({Vertex(0, 2), Vertex(0, 1), Vertex(1, 2)});
        intersections[6] = Intersection({Vertex(0, 1), Vertex(1, 2), Vertex(1, 1)});
        intersections[7] = Intersection({Vertex(1, 2), Vertex(1, 1), Vertex(2, 2)});
        intersections[8] = Intersection({Vertex(1, 1), Vertex(2, 2), Vertex(2, 1)});
        intersections[11] = Intersection({Vertex(-2, 0), Vertex(-1, 1), Vertex(-1, 0)});
        intersections[12] = Intersection({Vertex(-1, 1), Vertex(-1, 0), Vertex(0, 1)});
        intersections[13] = Intersection({Vertex(-1, 0), Vertex(0, 1), Vertex(0, 0)});
        intersections[14] = Intersection({Vertex(0, 1), Vertex(0, 0), Vertex(1, 1)});
        intersections[15] = Intersection({Vertex(0, 0), Vertex(1, 1), Vertex(1, 0)});
        intersections[16] = Intersection({Vertex(1, 1), Vertex(1, 0), Vertex(2, 1)});
        intersections[17] = Intersection({Vertex(1, 0), Vertex(2, 1), Vertex(2, 0)});

        // Three-vertex intersections: central-bottom part of the board
        intersections[20] = Intersection({Vertex(-2, 0), Vertex(-2, -1), Vertex(-1, 0)});
        intersections[21] = Intersection({Vertex(-2, -1), Vertex(-1, 0), Vertex(-1, -1)});
        intersections[22] = Intersection({Vertex(-1, 0), Vertex(-1, -1), Vertex(0, 0)});
        intersections[23] = Intersection({Vertex(-1, -1), Vertex(0, 0), Vertex(0, -1)});
        intersections[24] = Intersection({Vertex(0, 0), Vertex(0, -1), Vertex(1, 0)});
        intersections[25] = Intersection({Vertex(-1, 0), Vertex(1, 0), Vertex(1, -1)});
        intersections[26] = Intersection({Vertex(1, 0), Vertex(1, -1), Vertex(2, 0)});
        intersections[29] = Intersection({Vertex(-2, -1), Vertex(-2, -2), Vertex(-1, -1)});
        intersections[30] = Intersection({Vertex(-2, -2), Vertex(-1, -1), Vertex(-1, -2)});
        intersections[31] = Intersection({Vertex(-1, -1), Vertex(-1, -2), Vertex(0, -1)});
        intersections[32] = Intersection({Vertex(-1, -2), Vertex(0, -1), Vertex(0, -2)});
        intersections[33] = Intersection({Vertex(0, -1), Vertex(0, -2), Vertex(1, -1)});

        // Edge cases with only two vertices
        intersections[1] = Intersection({Vertex(0, 2), Vertex(1, 2)});
        intersections[2] = Intersection({Vertex(1, 2), Vertex(2, 2)});
        intersections[3] = Intersection({Vertex(-1, 1), Vertex(0, 2)});
        intersections[9] = Intersection({Vertex(2, 2), Vertex(2, 1)});
        intersections[10] = Intersection({Vertex(-2, 0), Vertex(-1, 1)});
        intersections[18] = Intersection({Vertex(2, 0), Vertex(2, 1)});
        intersections[19] = Intersection({Vertex(-2, -1), Vertex(-2, 0)});
        intersections[27] = Intersection({Vertex(2, 0), Vertex(1, -1)});
        intersections[28] = Intersection({Vertex(-2, 2), Vertex(-2, -1)});
        intersections[34] = Intersection({Vertex(1, -1), Vertex(0, -2)});
        intersections[35] = Intersection({Vertex(-1, -2), Vertex(-2, -2)});
        intersections[36] = Intersection({Vertex(0, -2), Vertex(-1, -2)});
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

    bool Board::canPlaceSettlement(int intersectionID, int playerID) 
    {
        // Ensure the intersection exists
        if (intersections.find(intersectionID) == intersections.end())
            return false;

        const auto& intersection = intersections[intersectionID];
        for (const Vertex& v : intersection.vertices) 
        {
            // Check if any vertex already has a settlement
            if (settlements.find(v) != settlements.end() && settlements[v].count(playerID)) 
            {
                return false;
            }
        }
        return true;  // If no settlements conflict, the placement is valid
    }

    void Board::placeSettlement(int intersectionID, int playerID) 
    {
        if (canPlaceSettlement(intersectionID, playerID)) {
            const auto& intersection = intersections[intersectionID];
            for (const Vertex& v : intersection.vertices) 
            {
                settlements[v].insert(playerID);  // Mark each vertex in the intersection as settled by this player
            }
            cout << "Player " << playerID << " placed a settlement at intersection ID " << intersectionID << "." << endl;
        } 
        else 
        {
            cout << "Settlement placement failed at intersection ID " << intersectionID << "." << endl;
        }
    }

    bool Board::canPlaceRoad(const Edge& edge, int playerID) 
    {
        // Check if the edge is already occupied by any road
        if (roads.find(edge) != roads.end() && roads[edge] == playerID) 
        {
            return false;  // Road already exists for this player
        }

        // Check if the edge is connected to a settlement or another road of the same player
        bool connected = (settlements.find(edge.v1) != settlements.end() && settlements[edge.v1].count(playerID)) ||
                        (settlements.find(edge.v2) != settlements.end() && settlements[edge.v2].count(playerID));

        // Check for adjacent roads of the same player
        if (!connected) 
        {
            for (const auto& [existingEdge, existingPlayerID] : roads) 
            {
                if (existingPlayerID == playerID && (existingEdge.v1 == edge.v1 || existingEdge.v1 == edge.v2 || 
                existingEdge.v2 == edge.v1 || existingEdge.v2 == edge.v2)) 
                {
                    connected = true;
                    break;
                }
            }
        }

        return connected;
    }

    void Board::placeRoad(const Edge& edge, int playerID) 
    {
        if (canPlaceRoad(edge, playerID)) 
        {
            roads[edge] = playerID;  // Assign the road to the player
            cout << "Player " << playerID << " placed a road between (" 
                << edge.v1.x << ", " << edge.v1.y << ") and (" 
                << edge.v2.x << ", " << edge.v2.y << ")." << endl;
        } 
        else 
        {
            cout << "Road placement failed between (" 
                << edge.v1.x << ", " << edge.v1.y << ") and (" 
                << edge.v2.x << ", " << edge.v2.y << ")." << endl;
        }
    }

}