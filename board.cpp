#include "board.hpp"
#include <iostream>
#include <cmath>


//              (0,2)   (1,2)   (2,2)
//         (-1,1)   (0,1)   (1,1)   (2,1)
//    (-2,0)   (-1,0)   (0,0)   (1,0)   (2,0)
//         (-2,-1)   (-1,-1)   (0,-1)   (1,-1)
//             (-2,-2)   (-1,-2)   (0,-2)


using namespace std;
namespace ariel {
    
    Board::Board() 
    { 
        setupTiles();               // Load tiles
        setupIntersections();       // Load all intersection of the all tiles
    }

    // Beginner fixed layout (assuming a 5x5 grid with the middle part filled as per Catan rules)
    void Board::setupTiles() 
    {
        // Row 1
        addTile({0, 2}, Tile(ResourceType::ORE, 10));
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
        addTile({-2, -2}, Tile(ResourceType::BRICK, 5));
        addTile({-1, -2}, Tile(ResourceType::GRAIN, 6));
        addTile({0, -2}, Tile(ResourceType::WOOL, 11));
    }

    void Board::setupIntersections() 
    {
        int id = 1;

        // Edge cases with only two vertices (1 to 3)
        intersections.insert({id++, Intersection({Vertex(0, 2), Vertex(1, 2)})});       
        intersections.insert({id++, Intersection({Vertex(1, 2), Vertex(2, 2)})});          
        intersections.insert({id++, Intersection({Vertex(-1, 1), Vertex(0, 2)})});         

        // Three-vertex intersections: central-upper part of the board (4 to 9)
        intersections.insert({id++, Intersection({Vertex(-1, 1), Vertex(0, 1), Vertex(0, 2)})});
        intersections.insert({id++, Intersection({Vertex(0, 2), Vertex(0, 1), Vertex(1, 2)})});
        intersections.insert({id++, Intersection({Vertex(0, 1), Vertex(1, 2), Vertex(1, 1)})});
        intersections.insert({id++, Intersection({Vertex(1, 2), Vertex(1, 1), Vertex(2, 2)})});
        intersections.insert({id++, Intersection({Vertex(1, 1), Vertex(2, 2), Vertex(2, 1)})});
        
        // Edge cases with only two vertices (9 to 10)
        intersections.insert({id++, Intersection({Vertex(2, 2), Vertex(2, 1)})});
        intersections.insert({id++, Intersection({Vertex(-2, 0), Vertex(-1, 1)})});
         
        // Three-vertex intersections: central-bottom part of the board (11 to 17)
        intersections.insert({id++, Intersection({Vertex(-2, 0), Vertex(-1, 1), Vertex(-1, 0)})});
        intersections.insert({id++, Intersection({Vertex(-1, 1), Vertex(-1, 0), Vertex(0, 1)})});
        intersections.insert({id++, Intersection({Vertex(-1, 0), Vertex(0, 1), Vertex(0, 0)})});
        intersections.insert({id++, Intersection({Vertex(0, 1), Vertex(0, 0), Vertex(1, 1)})});
        intersections.insert({id++, Intersection({Vertex(0, 0), Vertex(1, 1), Vertex(1, 0)})});
        intersections.insert({id++, Intersection({Vertex(1, 1), Vertex(1, 0), Vertex(2, 1)})});
        intersections.insert({id++, Intersection({Vertex(1, 0), Vertex(2, 1), Vertex(2, 0)})});

        // Edge cases with only two vertices (18 to 19)
        intersections.insert({id++, Intersection({Vertex(2, 0), Vertex(2, 1)})});
        intersections.insert({id++, Intersection({Vertex(-2, -1), Vertex(-2, 0)})});

        // Three-vertex intersections: central-bottom part of the board (20 to 26)
        intersections.insert({id++, Intersection({Vertex(-2, 0), Vertex(-2, -1), Vertex(-1, 0)})});
        intersections.insert({id++, Intersection({Vertex(-2, -1), Vertex(-1, 0), Vertex(-1, -1)})});
        intersections.insert({id++, Intersection({Vertex(-1, 0), Vertex(-1, -1), Vertex(0, 0)})});
        intersections.insert({id++, Intersection({Vertex(-1, -1), Vertex(0, 0), Vertex(0, -1)})});
        intersections.insert({id++, Intersection({Vertex(0, 0), Vertex(0, -1), Vertex(1, 0)})});
        intersections.insert({id++, Intersection({Vertex(-1, 0), Vertex(1, 0), Vertex(1, -1)})});
        intersections.insert({id++, Intersection({Vertex(1, 0), Vertex(1, -1), Vertex(2, 0)})});
        
        // Edge cases with only two vertices (27 to 28)
        intersections.insert({id++, Intersection({Vertex(2, 0), Vertex(1, -1)})});
        intersections.insert({id++, Intersection({Vertex(-2, 2), Vertex(-2, -1)})});

        // Three-vertex intersections: central-bottom part of the board (29 to 33)
        intersections.insert({id++, Intersection({Vertex(-2, -1), Vertex(-2, -2), Vertex(-1, -1)})});
        intersections.insert({id++, Intersection({Vertex(-2, -2), Vertex(-1, -1), Vertex(-1, -2)})});
        intersections.insert({id++, Intersection({Vertex(-1, -1), Vertex(-1, -2), Vertex(0, -1)})});
        intersections.insert({id++, Intersection({Vertex(-1, -2), Vertex(0, -1), Vertex(0, -2)})});
        intersections.insert({id++, Intersection({Vertex(0, -1), Vertex(0, -2), Vertex(1, -1)})});

        // Edge cases with only two vertices (34 to 36)
        intersections.insert({id++, Intersection({Vertex(1, -1), Vertex(0, -2)})});
        intersections.insert({id++, Intersection({Vertex(-1, -2), Vertex(-2, -2)})});
        intersections.insert({id++, Intersection({Vertex(0, -2), Vertex(-1, -2)})});
    }


    const Tile& Board::getTile(const std::pair<int, int>& position) const
    {
        return tiles.at(position);
    }

    Intersection Board::getIntersection(int intersectionID) const
    {
        auto it = intersections.find(intersectionID);
        if (it != intersections.end()) {
            return it->second;
        }
        throw std::out_of_range("Invalid intersection ID");
    }

    int Board::getIntersectionID(const Intersection& intersection) const {
        for (const auto& [id, inter] : intersections) {
            if (inter == intersection) {
                return id;
            }
        }
        throw std::out_of_range("Invalid intersection");
    }

    void Board::printBoard() const 
    {
        cout << "Board Layout:\n";
        for (int y = 2; y >= -2; --y) {
            for (int x = -2; x <= 2; x++) {
                if (tiles.find({x, y}) != tiles.end()) {
                    const Tile& tile = tiles.at({x, y});
                    string resource = resourceTypeToString(tile.getResourceType());
                    cout << resource[0] << "(" << tile.getNumber() << ") ";
                } else {
                    cout << "     ";  // Space for tiles that are not defined
                }
            }
            cout << "\n";
        }
    }

    string Board::tileString(const pair<int, int>& position) const {
        try 
        {
            const Tile& tile = getTile(position);
            std::string resourceInitial;
            switch (tile.getResourceType()) {
                case ResourceType::WOOD:    resourceInitial = "WOOD"; break;
                case ResourceType::BRICK:   resourceInitial = "BRICK"; break;
                case ResourceType::ORE:     resourceInitial = "ORE"; break;
                case ResourceType::GRAIN:   resourceInitial = "GRAIN"; break;
                case ResourceType::WOOL:    resourceInitial = "WOOL"; break;
                case ResourceType::NONE:    resourceInitial = "NONE"; break;    // Desert
                default:                    resourceInitial = "X"; break;       // Undefined
            }
            return resourceInitial + "(" + std::to_string(tile.getNumber()) + ")";
        } catch (const std::out_of_range&) {
            return "   ";  // Return spaces for non-existent tiles
        }
    }

    void Board::printBoardByTiles() const
    {
        std::vector<std::pair<int, int>> coordinates = {
        {0, 2}, {1, 2}, {2, 2},
        {-1, 1}, {0, 1}, {1, 1}, {2, 1}, 
        {-2, 0}, {-1, 0}, {0, 0}, {1, 0}, {2, 0},
        {-2, -1}, {-1, -1}, {0, -1}, {1, -1},
        {-2, -2}, {-1, -2}, {0, -2}};

        for (const auto& pos : coordinates) 
        {
            if (tiles.count(pos)) 
            {  
                const Tile& tile = tiles.at(pos);
                string resource = resourceTypeToString(tile.getResourceType());
                cout << "Tile at (" << pos.first << ", " << pos.second << ") - Resource: " << resource
                     << ", Number: " << tile.getNumber() << endl;
            } 
            else 
            {
                cout << "Tile at (" << pos.first << ", " << pos.second << ") - No tile" << endl;
            }
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
            return false;
        }

        // Check if the edge is connected to a settlement or another road of the same player
        bool connected = false;
        for (const auto& intersection : {edge.i1, edge.i2}) {
            for (const auto& vertex : intersection.vertices) {
                if (settlements.find(vertex) != settlements.end() && settlements[vertex].count(playerID)) {
                    connected = true;
                    break;
                }
            }
            if (connected) break;
        }

        // Check for adjacent roads of the same player
        if (!connected) 
        {
            for (const auto& [existingEdge, existingPlayerID] : roads) 
            {
                if (existingPlayerID == playerID && (existingEdge.i1 == edge.i1 || existingEdge.i1 == edge.i2 || 
                                                     existingEdge.i2 == edge.i1 || existingEdge.i2 == edge.i2)) 
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
        if (canPlaceRoad(edge, playerID)) {
            roads[edge] = playerID;     // Assign the road to the player
            cout << "Player " << playerID << " placed a road between intersections "
                << getIntersectionID(edge.i1) << " and " << getIntersectionID(edge.i2) << "." << endl;
        } else {
            cout << "Road placement failed between intersections "
                << getIntersectionID(edge.i1) << " and " << getIntersectionID(edge.i2) << "." << endl;
        }
    }

    // void Board::draw(sf::RenderWindow& window) {
    //     float radius = 40;  // radius of the hexagon

    //     for (auto& tilePair : tiles) {
    //         sf::ConvexShape hexagon(6);
    //         for (int i = 0; i < 6; ++i) {
    //             float angle = M_PI / 3.0f * i - M_PI / 6.0f;  // rotate hexagon
    //             hexagon.setPoint(i, sf::Vector2f(radius * cos(angle), radius * sin(angle)));
    //         }

    //         hexagon.setTexture(&textures[tilePair.second.getResourceType()]); // Set texture based on resource type

    //         int x = tilePair.first.first;
    //         int y = tilePair.first.second;

    //         // Adjust the positioning based on the tile coordinates
    //         float xOffset = 400 + radius * sqrt(3) * x;
    //         float yOffset = 300 + radius * 1.5f * y;

    //         hexagon.setPosition(xOffset, yOffset);

    //         sf::Text text;
    //         text.setFont(font);
    //         text.setString(std::to_string(tilePair.second.getNumber()));
    //         text.setCharacterSize(24); // Adjust size as needed
    //         text.setFillColor(sf::Color::White);
    //         sf::FloatRect textRect = text.getLocalBounds();
    //         text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    //         text.setPosition(hexagon.getPosition().x + radius * 0.5f, hexagon.getPosition().y + radius * 0.5f); // Position text in the center of the hexagon

    //         window.draw(hexagon);
    //         window.draw(text); // Draw the number on top of the hexagon
    //     }
    // }

    // void Board::loadResources() 
    // {
    //     // Load textures
    //     if (!textures[ResourceType::WOOD].loadFromFile("images/WOOD.png")) {
    //         std::cerr << "Error loading WOOD.png" << std::endl;
    //     }
    //     if (!textures[ResourceType::BRICK].loadFromFile("images/BRICK.png")) {
    //         std::cerr << "Error loading BRICK.png" << std::endl;
    //     }
    //     if (!textures[ResourceType::WOOL].loadFromFile("images/WOOL.png")) {
    //         std::cerr << "Error loading WOOL.png" << std::endl;
    //     }
    //     if (!textures[ResourceType::ORE].loadFromFile("images/ORE.png")) {
    //         std::cerr << "Error loading ORE.png" << std::endl;
    //     }
    //     if (!textures[ResourceType::NONE].loadFromFile("images/DESERT.png")) {
    //         std::cerr << "Error loading DESERT.png" << std::endl;
    //     }
    //     if (!textures[ResourceType::GRAIN].loadFromFile("images/GRAIN.png")) {
    //         std::cerr << "Error loading GRAIN.png" << std::endl;
    //     }

    //     if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf")) {
    //         std::cerr << "Error loading font!" << std::endl;
    //     }      
    // }

}