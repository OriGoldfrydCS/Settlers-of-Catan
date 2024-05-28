#include "board.hpp"
#include "player.hpp"  
#include <iostream>
#include <cmath>
#include <sstream>
#include <algorithm>



//              (0,2)   (1,2)   (2,2)
//         (-1,1)   (0,1)   (1,1)   (2,1)
//    (-2,0)   (-1,0)   (0,0)   (1,0)   (2,0)
//         (-2,-1)   (-1,-1)   (0,-1)   (1,-1)
//             (-2,-2)   (-1,-2)   (0,-2)


using namespace std;
namespace ariel {
    
    Board::Board() 
    { 
        Intersection::initialize();                  // Initiate intersections
        setupTiles();                                // Load tiles
        linkTilesAndIntersections();                 // Combine between each tile knows its intersections

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

    void Board::linkTilesAndIntersections() 
    {
        // Row 1
        tiles[{0, 2}].addIntersection(1);
        tiles[{0, 2}].addIntersection(2);
        tiles[{0, 2}].addIntersection(3);
        tiles[{0, 2}].addIntersection(9);
        tiles[{0, 2}].addIntersection(10);
        tiles[{0, 2}].addIntersection(11);

        tiles[{1, 2}].addIntersection(3);
        tiles[{1, 2}].addIntersection(4);
        tiles[{1, 2}].addIntersection(5);
        tiles[{1, 2}].addIntersection(11);
        tiles[{1, 2}].addIntersection(12);
        tiles[{1, 2}].addIntersection(13);


        tiles[{2, 2}].addIntersection(5);
        tiles[{2, 2}].addIntersection(6);
        tiles[{2, 2}].addIntersection(7);
        tiles[{2, 2}].addIntersection(13);
        tiles[{2, 2}].addIntersection(14);
        tiles[{2, 2}].addIntersection(15);

        // Row 2
        tiles[{-1, 1}].addIntersection(8);
        tiles[{-1, 1}].addIntersection(9);
        tiles[{-1, 1}].addIntersection(10);
        tiles[{-1, 1}].addIntersection(18);
        tiles[{-1, 1}].addIntersection(19);
        tiles[{-1, 1}].addIntersection(20);

        tiles[{0, 1}].addIntersection(10);
        tiles[{0, 1}].addIntersection(11);
        tiles[{0, 1}].addIntersection(12);
        tiles[{0, 1}].addIntersection(20);
        tiles[{0, 1}].addIntersection(21);
        tiles[{0, 1}].addIntersection(22);

        tiles[{1, 1}].addIntersection(12);
        tiles[{1, 1}].addIntersection(13);
        tiles[{1, 1}].addIntersection(14);
        tiles[{1, 1}].addIntersection(22);
        tiles[{1, 1}].addIntersection(23);
        tiles[{1, 1}].addIntersection(24);

        tiles[{2, 1}].addIntersection(14);
        tiles[{2, 1}].addIntersection(15);
        tiles[{2, 1}].addIntersection(16);
        tiles[{2, 1}].addIntersection(24);
        tiles[{2, 1}].addIntersection(25);
        tiles[{2, 1}].addIntersection(26);

        // Row 3
        tiles[{-2, 0}].addIntersection(17);
        tiles[{-2, 0}].addIntersection(18);
        tiles[{-2, 0}].addIntersection(19);
        tiles[{-2, 0}].addIntersection(28);
        tiles[{-2, 0}].addIntersection(29);
        tiles[{-2, 0}].addIntersection(30);


        tiles[{-1, 0}].addIntersection(19);
        tiles[{-1, 0}].addIntersection(20);
        tiles[{-1, 0}].addIntersection(21);
        tiles[{-1, 0}].addIntersection(30);
        tiles[{-1, 0}].addIntersection(31);
        tiles[{-1, 0}].addIntersection(32);

        tiles[{0, 0}].addIntersection(21);
        tiles[{0, 0}].addIntersection(22);
        tiles[{0, 0}].addIntersection(23);
        tiles[{0, 0}].addIntersection(32);
        tiles[{0, 0}].addIntersection(33);
        tiles[{0, 0}].addIntersection(34);

        tiles[{1, 0}].addIntersection(23);
        tiles[{1, 0}].addIntersection(24);
        tiles[{1, 0}].addIntersection(25);
        tiles[{1, 0}].addIntersection(34);
        tiles[{1, 0}].addIntersection(35);
        tiles[{1, 0}].addIntersection(36);

        tiles[{2, 0}].addIntersection(25);
        tiles[{2, 0}].addIntersection(26);
        tiles[{2, 0}].addIntersection(27);
        tiles[{2, 0}].addIntersection(36);
        tiles[{2, 0}].addIntersection(37);
        tiles[{2, 0}].addIntersection(38);

        // Row 3
        tiles[{-2, -1}].addIntersection(29);
        tiles[{-2, -1}].addIntersection(30);
        tiles[{-2, -1}].addIntersection(31);
        tiles[{-2, -1}].addIntersection(39);
        tiles[{-2, -1}].addIntersection(40);
        tiles[{-2, -1}].addIntersection(41);

        tiles[{-1, -1}].addIntersection(31);
        tiles[{-1, -1}].addIntersection(32);
        tiles[{-1, -1}].addIntersection(33);
        tiles[{-1, -1}].addIntersection(41);
        tiles[{-1, -1}].addIntersection(42);
        tiles[{-1, -1}].addIntersection(43);

        tiles[{0, -1}].addIntersection(33);
        tiles[{0, -1}].addIntersection(34);
        tiles[{0, -1}].addIntersection(35);
        tiles[{0, -1}].addIntersection(43);
        tiles[{0, -1}].addIntersection(44);
        tiles[{0, -1}].addIntersection(45);

        tiles[{1, -1}].addIntersection(35);
        tiles[{1, -1}].addIntersection(36);
        tiles[{1, -1}].addIntersection(37);
        tiles[{1, -1}].addIntersection(45);
        tiles[{1, -1}].addIntersection(46);
        tiles[{1, -1}].addIntersection(47);

        // Row 5
        tiles[{-2, -2}].addIntersection(40);
        tiles[{-2, -2}].addIntersection(41);
        tiles[{-2, -2}].addIntersection(42);
        tiles[{-2, -2}].addIntersection(48);
        tiles[{-2, -2}].addIntersection(49);
        tiles[{-2, -2}].addIntersection(50);

        tiles[{-1, -2}].addIntersection(42);
        tiles[{-1, -2}].addIntersection(43);
        tiles[{-1, -2}].addIntersection(44);
        tiles[{-1, -2}].addIntersection(50);
        tiles[{-1, -2}].addIntersection(51);
        tiles[{-1, -2}].addIntersection(52);

        tiles[{0, -2}].addIntersection(44);
        tiles[{0, -2}].addIntersection(45);
        tiles[{0, -2}].addIntersection(46);
        tiles[{0, -2}].addIntersection(52);
        tiles[{0, -2}].addIntersection(53);
        tiles[{0, -2}].addIntersection(54);
    }

    const Tile& Board::getTile(const std::pair<int, int>& position) const
    {
        return tiles.at(position);
    }

    // Intersection Board::getIntersection(int intersectionID) const
    // {
    //     auto it = intersections.find(intersectionID);
    //     if (it != intersections.end()) {
    //         return it->second;
    //     }
    //     throw std::out_of_range("Invalid intersection ID");
    // }

    int Board::getIntersectionID(const Intersection& intersection) const {
        const auto& allIntersections = Intersection::getAllIntersections();
        for (const auto& [id, inter] : allIntersections) {
            if (inter == intersection) {
                return id;
            }
        }
        throw std::out_of_range("Intersection not found");
    }

    void Board::printAllBoardToCheckTileIntersections() const {
        const auto& allIntersections = Intersection::getAllIntersections(); // Fetch all intersections once, to use below
        set<int> printedIntersections;
        set<pair<int, int>> printedRoads; // Using pair of intersection IDs to track printed roads

        for (const auto& [pos, tile] : tiles) {
            std::cout << "Tile at (" << pos.first << ", " << pos.second << "): "
                    << resourceTypeToString(tile.getResourceType()) << " Number: " << tile.getNumber() << std::endl;

            // Get the intersections IDs associated with the tile
            const auto& intersectionIDs = tile.getIntersectionIDs();
            for (const auto& id : intersectionIDs) {
                if (printedIntersections.insert(id).second) { // Check if this intersection has already been printed
                    // Print the intersection details
                    const auto& intersection = allIntersections.at(id); // Use at() to access the intersection directly
                    std::cout << "  Intersection " << id << " includes vertices: ";
                    for (const auto& vertex : intersection.vertices) {
                        std::cout << vertex << " ";
                    }
                    std::cout << std::endl;

                    // Check for settlements
                    if (settlements.count(id) > 0) {
                        for (const auto& playerID : settlements.at(id)) {
                            std::cout << "    Settlement by Player " << playerID << std::endl;
                        }
                    }
                }

                // Check for roads
                for (const auto& [edge, playerID] : roads) {
                    int startID = getIntersectionID(edge.i1);
                    int endID = getIntersectionID(edge.i2);
                    if ((edge.involvesIntersection(id, allIntersections)) && printedRoads.insert({min(startID, endID), max(startID, endID)}).second) {
                        std::cout << "    Road by Player " << playerID << " between "
                                << startID << " and " << endID << std::endl;
                    }
                }
            }
        }
    }


    void Board::printBoard() const 
    {
        for (const auto& [pos, tile] : tiles) 
        {
            const auto& intersectionIDs = tile.getIntersectionIDs();
            std::vector<std::string> features; // To collect features of the tile

            // Check for settlements at each intersection
            for (const auto& id : intersectionIDs) {
                if (settlements.find(id) != settlements.end() && !settlements.at(id).empty()) {
                    std::string settlementInfo = "    Settlements by Players ";
                    for (const auto& playerID : settlements.at(id)) {
                        settlementInfo += std::to_string(playerID) + " ";
                    }
                    settlementInfo += "on Intersection " + std::to_string(id);
                    features.push_back(settlementInfo);
                }
            }

            // Check for roads connected to intersections of this tile
            for (const auto& [edge, playerID] : roads) {
                if ((std::find(intersectionIDs.begin(), intersectionIDs.end(), edge.getId1()) != intersectionIDs.end()) &&
                    (std::find(intersectionIDs.begin(), intersectionIDs.end(), edge.getId2()) != intersectionIDs.end())) {
                    features.push_back("    Road by Player " + std::to_string(playerID) + 
                                    " between " + std::to_string(edge.getId1()) + " and " + std::to_string(edge.getId2()));
                }
            }

            // Print the tile and its features if there are any
            if (!features.empty()) {
                cout << "*******************************************************" << endl;
                std::cout << resourceTypeToString(tile.getResourceType()) << " " << tile.getNumber()
                << ", Tile at (" << pos.first << ", " << pos.second << "): " << std::endl;

                for (const auto& feature : features) {
                    std::cout << feature << std::endl;
                }
            }
        }

        cout << "*******************************************************" << endl;

    }

    string Board::tileString(const pair<int, int>& position) const {
        try 
        {
            const Tile& tile = getTile(position);
            std::string resourceInitial;
            switch (tile.getResourceType()) 
            {
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


    vector<ResourceType> Board::getResourceTypesAroundIntersection(int intersectionID) {
        set<ResourceType> uniqueResources; // Use a set to ensure uniqueness
        set<pair<int, int>> contributingTiles; // To track which tiles contribute resources

        for (const auto& [position, tile] : tiles) {
            const auto& ids = tile.getIntersectionIDs();
            if (ids.find(intersectionID) != ids.end()) {
                ResourceType type = tile.getResourceType();
                if (type != ResourceType::NONE) {  // Exclude deserts
                    uniqueResources.insert(type);
                    contributingTiles.insert(position); // Debug: Track contributing tile positions
                }
            }
        }

        // Debug output to see contributing tiles
        cout << "Intersection " << intersectionID << " receives resources from tiles at: ";
        for (const auto& pos : contributingTiles) {
            cout << "(" << pos.first << ", " << pos.second << ") ";
        }
        cout << endl;

        // Convert set back to vector for return type compliance or other needs
        vector<ResourceType> resources(uniqueResources.begin(), uniqueResources.end());
        return resources;
    }




    void Board::distributeResourcesBasedOnDiceRoll(int diceRoll, Player* currentPlayer) {
    cout << "Distributing resources for dice roll: " << diceRoll << "." << endl;

    // This will collect all resource options available for the dice roll
    map<ResourceType, vector<pair<int, int>>> resourceOptions;

    // Gather all tiles that match the dice roll
    for (auto& [position, tile] : tiles) {
        if (tile.getNumber() == diceRoll && tile.getResourceType() != ResourceType::NONE) {
            resourceOptions[tile.getResourceType()].push_back(position);
        }
    }

    // Present resource options to the player if more than one type is available
    if (resourceOptions.empty()) {
        cout << "No resources available for this roll." << endl;
    } else if (resourceOptions.size() == 1 && resourceOptions.begin()->second.size() == 1) {
        // If only one type of resource from one tile, directly add it
        auto& resourceType = resourceOptions.begin()->first;
        currentPlayer->addResource(resourceType, 1);
        cout << "You received 1 " << resourceTypeToString(resourceType) << " from tile at (" 
             << resourceOptions.begin()->second[0].first << ", " 
             << resourceOptions.begin()->second[0].second << ")." << endl;
    } else {
        // More than one resource type or more than one tile for the same resource
        cout << "Choose which resource to collect:" << endl;
        int optionIndex = 1;
        vector<ResourceType> choices;
        for (auto& [resourceType, positions] : resourceOptions) {
            for (auto& pos : positions) {
                cout << "Option " << optionIndex++ << ": " << resourceTypeToString(resourceType) 
                     << " from tile at (" << pos.first << ", " << pos.second << ")" << endl;
                choices.push_back(resourceType);
            }
        }
        
        int selectedOption;
        cout << "Enter option number: ";
        cin >> selectedOption;
        if (selectedOption > 0 && selectedOption <= choices.size()) {
            currentPlayer->addResource(choices[selectedOption - 1], 1);
            cout << "You received 1 " << resourceTypeToString(choices[selectedOption - 1]) << "." << endl;
        } else {
            cout << "Invalid option selected." << endl;
        }
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

    bool Board::canPlaceSettlement(int intersectionID, int playerID) {
        // Access all intersections via the Intersection class
        const auto& allIntersections = Intersection::getAllIntersections();

        // Ensure the intersection exists
        if (allIntersections.find(intersectionID) == allIntersections.end())
            return false;

        // Check if the intersection already has a settlement by the player
        if (settlements.find(intersectionID) != settlements.end() && settlements[intersectionID].count(playerID))
        {
            return false;
        }
        return true;  // If no settlements conflict, the placement is valid
    }


    void Board::placeSettlement(int intersectionID, int playerID) {
        if (canPlaceSettlement(intersectionID, playerID)) {
            settlements[intersectionID].insert(playerID);
            std::cout << "Player " << playerID << " placed a settlement at intersection ID " << intersectionID << "." << std::endl;
        } else {
            std::cout << "Settlement placement failed at intersection ID " << intersectionID << "." << std::endl;
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
            int intersectionID = getIntersectionID(intersection);
            if (settlements.find(intersectionID) != settlements.end() && settlements[intersectionID].count(playerID)) {
                connected = true;
                break;
            }
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

    // Check if an intersection has any settlements
    bool Board::hasSettlement(int intersectionID) 
    {
        return settlements.find(intersectionID) != settlements.end() && !settlements[intersectionID].empty();
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