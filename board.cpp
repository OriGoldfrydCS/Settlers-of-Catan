// Email: origoldbsc@gmail.com

#include "board.hpp"
#include <iostream>
#include <cmath>
#include <sstream>
#include <algorithm>

//-------------------------------------------------------//
//   The tiles represented by the coordinates below:     //
//                                                       //
//               (0,2)   (1,2)   (2,2)                   //
//          (-1,1)   (0,1)   (1,1)   (2,1)               //
//       (-2,0)  (-1,0)  (0,0)  (1,0)    (2,0)           //
//          (-2,-1)  (-1,-1) (0,-1)  (1,-1)              //
//              (-2,-2) (-1,-2)  (0,-2)                  //
//                                                       //
//-------------------------------------------------------//


using namespace std;
namespace ariel {
    
    /**
     * @brief Static member to store adjacency list for intersections on the board.
     */
    map<int, set<int>> Board::adjacencyList;  


    /**
     * @brief Returns the singleton instance of the Board.
     * @return Reference to the singleton instance of the Board.
     */
    Board& Board::getInstance() 
    {
        static Board instance;
        return instance;
    }
    
    /**
     * @brief Constructs a new game board by initializing intersections, setting up tiles, and linking tiles with their respective intersections.
     */
    Board::Board() 
    { 
        Intersection::initialize();                  // Initiate intersections
        setupTiles();                                // Load tiles
        linkTilesAndIntersections();                 // Combine between each tile knows its intersections
        initializeAdjacency();                       // Initialize the adjacency list for intersections
    }

    /**
     * @brief Sets up the tiles in a predefined pattern based on the standard board layout of Catan.
     */
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


    /**
     * @brief Links tiles to their adjacent intersections to facilitate game mechanics like resource distribution.
     */
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
    

    /**
     * @brief Initializes the adjacency list used to check the connectivity between intersections.
     */
    void Board::initializeAdjacency() {
        adjacencyList[1] = {2, 9};
        adjacencyList[2] = {1, 3};
        adjacencyList[3] = {2, 4, 11};
        adjacencyList[4] = {3, 5};
        adjacencyList[5] = {4, 6, 13};
        adjacencyList[6] = {5, 7};
        adjacencyList[7] = {6, 15};
        adjacencyList[8] = {9, 18};
        adjacencyList[9] = {1, 8, 10};
        adjacencyList[10] = {9, 11, 20};
        adjacencyList[11] = {3, 10, 12};
        adjacencyList[12] = {11, 13, 22};
        adjacencyList[13] = {5, 12, 14};
        adjacencyList[14] = {13, 15, 24};
        adjacencyList[15] = {7, 14, 16};
        adjacencyList[16] = {15, 26};
        adjacencyList[17] = {18, 28};
        adjacencyList[18] = {8, 17, 19};
        adjacencyList[19] = {18, 20, 30};
        adjacencyList[20] = {10, 19, 21};
        adjacencyList[21] = {20, 22, 32};
        adjacencyList[22] = {12, 21, 23};
        adjacencyList[23] = {22, 24, 34};
        adjacencyList[24] = {14, 23, 25};
        adjacencyList[25] = {24, 26, 36};
        adjacencyList[26] = {16, 25, 27};
        adjacencyList[27] = {26, 38};
        adjacencyList[28] = {17, 29};
        adjacencyList[29] = {28, 30, 39};
        adjacencyList[30] = {19, 29, 31};
        adjacencyList[31] = {30, 32, 41};
        adjacencyList[32] = {21, 31, 33};
        adjacencyList[33] = {32, 34, 43};
        adjacencyList[34] = {23, 33, 35};
        adjacencyList[35] = {34, 36, 45};
        adjacencyList[36] = {25, 35, 37};
        adjacencyList[37] = {36, 38, 47};
        adjacencyList[38] = {27, 37};
        adjacencyList[39] = {29, 40};
        adjacencyList[40] = {39, 41, 48};
        adjacencyList[41] = {31, 40, 42};
        adjacencyList[42] = {41, 43, 50};
        adjacencyList[43] = {33, 42, 44};
        adjacencyList[44] = {43, 45, 52};
        adjacencyList[45] = {35, 44, 46};
        adjacencyList[46] = {45, 47, 54};
        adjacencyList[47] = {37, 46};
        adjacencyList[48] = {40, 49};
        adjacencyList[49] = {48, 50};
        adjacencyList[50] = {42, 49, 51};
        adjacencyList[51] = {50, 52};
        adjacencyList[52] = {44, 51, 53};
        adjacencyList[53] = {52, 54};
        adjacencyList[54] = {46, 53};
    }


    /**
     * @brief Determines if two intersections are adjacent based on the board's layout.
     * @param id1 First intersection ID.
     * @param id2 Second intersection ID.
     * @return true If intersections are adjacent, false otherwise.
     */
    bool Board::areIntersectionsAdjacent(int id1, int id2) 
    {
        // Check if one of the intersections contains the other in its adjacency list
        auto it = adjacencyList.find(id1);
        if (it != adjacencyList.end()) 
        {
            return it->second.find(id2) != it->second.end();
        }
        return false;
    }


    /**
     * @brief Returns a tile from a specific board position.
     * @param position The position of the tile in a grid layout.
     * @return const reference to the Tile object.
     */
    const Tile& Board::getTile(const pair<int, int>& position) const
    {
        return tiles.at(position);
    }

   
    /**
     * @brief Returns the unique identifier for a given intersection based on its object reference.
     * @param intersection The reference to the intersection object.
     * @return int The unique identifier of the intersection.
     * @throw out_of_range If the intersection is not found.
     */
    int Board::getIntersectionID(const Intersection& intersection) const 
    {
        const auto& allIntersections = Intersection::getAllIntersections();
        for (const auto& [id, inter] : allIntersections) {
            if (inter == intersection) 
            {
                return id;
            }
        }
        throw out_of_range("Invalid intersection ID");
    }


    /**
     * @brief Prints all tiles and their respective intersections to verify correct setup and debugging.
     */
    void Board::printAllBoardToCheckTileIntersections() const 
    {
        const auto& allIntersections = Intersection::getAllIntersections(); 
        set<int> printedIntersections;
        set<pair<int, int>> printedRoads;       // Using pair of intersection IDs to track printed roads

        for (const auto& [pos, tile] : tiles) 
        {
            cout << "Tile at (" << pos.first << ", " << pos.second << "): "
            << resourceTypeToString(tile.getResourceType()) << " Number: " << tile.getNumber() << endl;

            // Get the intersections IDs associated with the tile
            const auto& intersectionIDs = tile.getIntersectionIDs();
            for (const auto& id : intersectionIDs) 
            {   
                // Check if this intersection has already been printed
                if (printedIntersections.insert(id).second) 
                { 
                    // Print the intersection details
                    const auto& intersection = allIntersections.at(id);         // I use at() to access the intersection directly
                    cout << "  Intersection " << id << " includes vertices: ";
                    for (const auto& vertex : intersection.vertices) 
                    {
                        cout << vertex << " ";
                    }
                    cout << endl;

                    // Check for settlements
                    if (settlements.count(id) > 0) 
                    {
                        for (const auto& playerID : settlements.at(id)) 
                        {
                            cout << "    Settlement by Player " << playerID << endl;
                        }
                    }
                }

                // Check for roads
                for (const auto& [edge, playerID] : roads) 
                {
                    int startID = getIntersectionID(edge.getIntersection1());
                    int endID = getIntersectionID(edge.getIntersection2());
                    if ((edge.involvesIntersection(id, allIntersections)) && printedRoads.insert({min(startID, endID), max(startID, endID)}).second) 
                    {
                        cout << "    Road by Player " << playerID << " between "
                        << startID << " and " << endID << endl;
                    }
                }
            }
        }
    }

    /**
     * @brief Prints a visual representation of the entire board, showing tiles, roads, settlements, and cities.
     */
    void Board::printBoard() const 
    {
        for (const auto& [pos, tile] : tiles) 
        {
            const auto& intersectionIDs = tile.getIntersectionIDs();
            vector<string> features; // To collect features of the tile

            // Check for settlements at each intersection
            for (const auto& id : intersectionIDs) 
            {
                if (settlements.find(id) != settlements.end() && !settlements.at(id).empty()) 
                {
                    string settlementInfo = "    Settlements by Players ";
                    for (const auto& playerID : settlements.at(id)) 
                    {
                        settlementInfo += to_string(playerID) + " ";
                    }
                    settlementInfo += "on Intersection " + to_string(id);
                    features.push_back(settlementInfo);
                }

                // Check for cities at each intersection
                auto cityIter = cities.find(id);
                if (cityIter != cities.end()) {
                    string cityInfo = "    City by Player " + to_string(cityIter->second) + " on Intersection " + to_string(id);
                    features.push_back(cityInfo);
                }
            }

            // Check for roads connected to intersections of this tile
            for (const auto& [edge, playerID] : roads) 
            {
                if ((find(intersectionIDs.begin(), intersectionIDs.end(), edge.getId1()) != intersectionIDs.end()) &&
                (find(intersectionIDs.begin(), intersectionIDs.end(), edge.getId2()) != intersectionIDs.end())) 
                {
                    features.push_back("    Road by Player " + to_string(playerID) + 
                                    " between " + to_string(edge.getId1()) + " and " + to_string(edge.getId2()));
                }
            }

            // Print the tile and its features if there are any
            if (!features.empty()) 
            {
                cout << "*******************************************************" << endl;
                cout << resourceTypeToString(tile.getResourceType()) << " " << tile.getNumber()
                << ", Tile at (" << pos.first << ", " << pos.second << "): " << endl;

                for (const auto& feature : features) 
                {
                    cout << feature << endl;
                }
            }
        }

        cout << "*******************************************************" << endl;
    }


    /**
     * @brief Converts a board position into a string representation for display.
     * @param position The position of the tile.
     * @return A string representing the resource and number on the tile.
     */
    string Board::tileString(const pair<int, int>& position) const 
    {
        try 
        {
            const Tile& tile = getTile(position);
            string resourceInitial;
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
            return resourceInitial + "(" + to_string(tile.getNumber()) + ")";
        } 
        catch (const out_of_range&) 
        {
            return "   ";  // Return spaces for non-existent tiles
        }
    }

    /**
     * @brief Returns a list of unique resource types surrounding a specific intersection.
     * @param intersectionID The ID of the intersection.
     * @return vector of ResourceType Enumerations surrounding the given intersection.
     */
    vector<ResourceType> Board::getResourceTypesAroundIntersection(int intersectionID) 
    {
        set<ResourceType> uniqueResources;          // Use a set to ensure uniqueness
        set<pair<int, int>> contributingTiles;      // To track which tiles contribute resources

        for (const auto& [position, tile] : tiles) 
        {
            const auto& ids = tile.getIntersectionIDs();
            if (ids.find(intersectionID) != ids.end()) 
            {
                ResourceType type = tile.getResourceType();
                // Exclude deserts
                if (type != ResourceType::NONE) 
                {  
                    uniqueResources.insert(type);
                    contributingTiles.insert(position);     // Debug: Track contributing tile positions
                }
            }
        }

        // Convert set back to vector for return type compliance or other needs
        vector<ResourceType> resources(uniqueResources.begin(), uniqueResources.end());
        return resources;
    }


    /**
     * @brief Distributes resources to players based on the dice roll matching the number on the tiles.
     * @param diceRoll The result of the dice roll.
     * @param players Vector containing pointers to all players in the game.
     */
    void Board::distributeResourcesBasedOnDiceRoll(int diceRoll, const vector<Player*>& players) 
    {
        cout << "Distributing resources for dice roll: " << diceRoll << "." << endl;

        // Mapping from ResourceType to players who should receive that resource this turn
        map<Player*, map<ResourceType, int>> resourcesToDistribute;

        // Find all tiles that match the dice roll number and are not "NONE"
        for (auto& [position, tile] : tiles) 
        {
            if (tile.getNumber() == diceRoll && tile.getResourceType() != ResourceType::NONE) 
            {
                // Get all intersections around this tile
                const auto& intersectionIDs = tile.getIntersectionIDs();
                for (int intersectionID : intersectionIDs) 
                {
                    // Check each player if they have a settlement or city on this intersection
                    for (auto* player : players) 
                    {
                        if (player->getSettlements().find(intersectionID) != player->getSettlements().end()) 
                        {
                            // If the player has a settlement here, they should receive resources from this tile
                            resourcesToDistribute[player][tile.getResourceType()] += 1;
                        }
                        if (player->getCities().find(intersectionID) != player->getCities().end()) 
                        {
                            // If the player has a city here, they should receive double resources from this tile
                            resourcesToDistribute[player][tile.getResourceType()] += 2;  // Double the resources for cities
                        }
                    }
                }
            }
        }

        // Distribute the resources to players
        for (auto& [player, resources] : resourcesToDistribute) 
        {
            for (auto& [resourceType, quantity] : resources) 
            {
                player->addResource(resourceType, quantity);  // Distribute the calculated resource quantity
                cout << "Player " << player->getName() << " received " << quantity << " " << resourceTypeToString(resourceType) << "." << endl;
            }
        }
    }


    /**
     * @brief Returns all tiles that surround a particular intersection.
     * @param intersectionID The ID of the intersection being checked.
     * @return vector of Tile objects around the specified intersection.
     */ 
    vector<Tile> Board::getTilesAroundIntersection(int intersectionID) const 
    {
        vector<Tile> surroundingTiles;
        // Iterate through all tiles and check if they include the intersection
        for (const auto& [position, tile] : tiles) 
        {
            if (find(tile.getIntersectionIDs().begin(), tile.getIntersectionIDs().end(), intersectionID) != tile.getIntersectionIDs().end()) 
            {
                surroundingTiles.push_back(tile);
            }
        }
        return surroundingTiles;
    }


    /**
     * @brief Displays the board's tiles in the command line interface with details about resources and numbers (for debug).
     */
    void Board::printBoardByTiles() const
    {
        vector<pair<int, int>> coordinates = {
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


    /**
     * @brief Adds a tile to the board at a specified position.
     * @param position The position to place the tile.
     * @param tile The tile object to place.
     */
    void Board::addTile(const pair<int, int>& position, const Tile& tile)
    {
        tiles[position] = tile;
    }


    /**
     * @brief Checks if a tile position is currently unoccupied.
     * @param position The position to check.
     * @return true If there is no tile at the position, false if there is a tile.
     */
    bool Board::isTileAvailable(const pair<int, int>& position) const
    {
        return tiles.find(position) == tiles.end();
    }


     /**
     * @brief Places an initial settlement at a specified intersection for a player.
     * This function is  used during the setup phase of the game where initial settlements are placed without the usual game constraints.
     * @param intersectionID The intersection ID where the settlement is to be placed.
     * @param playerID The identifier of the player placing the settlement.
     */
    void Board::placeInitialSettlement(int intersectionID, int playerID) 
    {
        // Simply place the settlement without any checks for resources or surrounding settlements
        settlements[intersectionID].insert(playerID);
        cout << "Player " << playerID << " placed an initial settlement at intersection " << intersectionID << "." << endl;
    }


    /**
     * @brief Places an initial road between two intersections for a player.
     * This function is  used during the setup phase to place initial roads without checking for resource costs or other gameplay constraints.
     * @param edge The edge representing the road to be placed.
     * @param playerID The identifier of the player placing the road.
     */
    void Board::placeInitialRoad(const Edge& edge, int playerID) 
    {
        // Place the road without checking for resources or connectivity to other roads
        roads.insert({edge, playerID});
        cout << "Player " << playerID << " placed an initial road between intersections " 
        << getIntersectionID(edge.getIntersection1()) << " and " << getIntersectionID(edge.getIntersection2()) << "." << endl;
    }


    /**
     * @brief Checks if a settlement can be legally placed at a given intersection by a player.
     * This function checks if the intersection exists, is connected to a player's road, and is not adjacent to other settlements.
     * @param intersectionID The intersection ID where the settlement is to be placed.
     * @param playerID The identifier of the player attempting to place the settlement.
     * @return true If the settlement can be legally placed, false otherwise.
     */
    bool Board::canPlaceSettlement(int intersectionID, int playerID) 
    {
        // Ensure the intersection exists and is connected to a road owned by the player
        const auto& allIntersections = Intersection::getAllIntersections();
        if (allIntersections.find(intersectionID) == allIntersections.end() || !isIntersectionConnectedToPlayerRoad(intersectionID, playerID)) 
        {
            cout << "Cannot place settlement: no road connection or invalid intersection." << endl;
            return false;
        }

        // Check for settlements too close to the desired location
        for (int neighbor : adjacencyList[intersectionID])
        {
            if (hasSettlement(neighbor)) 
            {
                cout << "Cannot place settlement: too close to another settlement." << endl;
                return false;
            }
        }

        return true; // Placement is valid
    }


    /**
     * @brief Checks if an intersection is connected to any of a player's roads.
     * This function ensures connectivity between an intersection and the roads owned by a player.
     * @param intersectionID The intersection ID to check.
     * @param playerID The identifier of the player.
     * @return true If the intersection is connected to any of the player's roads, false otherwise.
     */
    bool Board::isIntersectionConnectedToPlayerRoad(int intersectionID, int playerID) 
    {
        for (const auto& [edge, ownerID] : roads) 
        {
            if (ownerID == playerID && (edge.getId1() == intersectionID || edge.getId2() == intersectionID)) 
            {
                return true;
            }
        }
        return false;
    }


    /**
     * @brief Places a settlement at a specified intersection for a player.
     * This function places a settlement if all game conditions are met, such as no adjacent settlements
     * and a connecting road.
     * @param intersectionID The intersection ID where the settlement is to be placed.
     * @param playerID The identifier of the player placing the settlement.
     */
    void Board::placeSettlement(int intersectionID, int playerID) 
    {
        if (canPlaceSettlement(intersectionID, playerID)) 
        {
            settlements[intersectionID].insert(playerID);
            cout << "Player " << playerID << " placed a settlement at intersection " << intersectionID << "." << endl;
        } 
        else 
        {
            cout << "Settlement placement failed at intersection " << intersectionID << "." << endl;
        }
    }

    bool Board::canPlaceRoad(const Edge& newRoad, int playerID) 
    {
        // Check if the road is already present
        if (roads.find(newRoad) != roads.end()) 
        {
            return false;
        }

        // Check if the intersections are adjacent
        if (!areIntersectionsAdjacent(newRoad.getId1(), newRoad.getId2())) 
        {
            cout << "Cannot place road: intersections are not neighbors." << endl;
            return false;
        }

        // Check for connection to the player's settlements or existing roads
        bool isConnected = false;
        if ((settlements.find(newRoad.getId1()) != settlements.end() && settlements[newRoad.getId1()].count(playerID)) ||
        (settlements.find(newRoad.getId2()) != settlements.end() && settlements[newRoad.getId2()].count(playerID))) 
        {
            isConnected = true;
        }
        
        for (const auto& [existingEdge, existingPlayerID] : roads) 
        {
            if (existingPlayerID == playerID && (existingEdge.involvesIntersection(newRoad.getId1()) || existingEdge.involvesIntersection(newRoad.getId2()))) 
            {
                isConnected = true;
                break;
            }
        }
        return isConnected;
    }



    /**
     * @brief Places a road between two intersections for a player.
     * This function places a road if all game conditions are met, such as adjacency of intersections and connectivity to existing structures.
     * @param edge The edge representing the road to be placed.
     * @param playerID The identifier of the player placing the road.
     */
    void Board::placeRoad(const Edge& edge, int playerID) 
    {
        if (canPlaceRoad(edge, playerID)) 
        {
            roads[edge] = playerID;     // Assign the road to the player
            // cout << "Player " << playerID << " placed a road between intersections "
            // << getIntersectionID(edge.getIntersection1()) << " and " << getIntersectionID(edge.getIntersection2()) << "." << endl;
        } 
        else 
        {
            cout << "Road placement failed between intersections "
            << getIntersectionID(edge.getIntersection1()) << " and " << getIntersectionID(edge.getIntersection2()) << "." << endl;
        }
    }

    /**
     * @brief Checks if a settlement at a specific intersection can be upgraded to a city by a player.
     * This function checks if a player has a settlement at the specified intersection and if the game conditions permit an upgrade.
     * @param intersectionID The intersection ID where the city upgrade is attempted.
     * @param playerID The identifier of the player attempting the upgrade.
     * @return true If the city can be upgraded, false otherwise.
     */
    bool Board::canUpgradeSettlementToCity(int intersectionID, int playerID) 
    {
        // Check if there's a settlement belonging to the player at the specified intersection
        auto it = settlements.find(intersectionID);
        if (it != settlements.end() && it->second.count(playerID) > 0) 
        {
            // Check if the player has enough resources to upgrade (handled in Player class)
            return true;
        }
        return false;
    }


      /**
     * @brief Upgrades a settlement at a specified intersection to a city for a player.
     * This function upgrades a settlement to a city if all game conditions are met.
     * @param intersectionID The intersection ID where the city is to be upgraded.
     * @param playerID The identifier of the player upgrading the city.
     */
    void Board::upgradeSettlementToCity(int intersectionID, int playerID) 
    {
        if (canUpgradeSettlementToCity(intersectionID, playerID)) 
        {
            // Remove settlement entry
            settlements[intersectionID].erase(playerID);
            if (settlements[intersectionID].empty()) 
            {
                settlements.erase(intersectionID);
            }

            // Add to cities
            cities[intersectionID] = playerID;

            cout << "Player " << playerID << " upgraded a settlement to a city at intersection " << intersectionID << "." << endl;
        } 
        else 
        {
            cout << "Cannot upgrade to city at " << intersectionID << endl;
        }
    }


    /**
     * @brief Checks if an intersection has any settlements.
     * @param intersectionID The intersection ID to check.
     * @return true If there is at least one settlement at the intersection, false otherwise.
     */
    bool Board::hasSettlement(int intersectionID) 
    {
        return settlements.find(intersectionID) != settlements.end() && !settlements[intersectionID].empty();
    }


    /**
     * @brief Returns a map of cities and their respective owners.
     * @return map linking intersections with player IDs who own cities there.
     */
    const map<int, int>& Board::getCities() const 
    {
        return cities;
    }


     /**
     * @brief Checks if there is a road between two intersections for testing purposes.
     * @param intersectionID1 The first intersection ID.
     * @param intersectionID2 The second intersection ID.
     * @return true If there is a road between the intersections, false otherwise.
     */
    bool Board::isRoadPresent(int intersectionID1, int intersectionID2) const 
    {
        Edge road1(Intersection::getIntersection(intersectionID1), Intersection::getIntersection(intersectionID2));
        Edge road2(Intersection::getIntersection(intersectionID2), Intersection::getIntersection(intersectionID1));
        return roads.find(road1) != roads.end() || roads.find(road2) != roads.end();
    }

    
    /**
     * @brief Prints a visual representation of the game board, showing tiles, roads, settlements, and cities.
     */
    void Board::printGameBoard() const {
        map<int, string> IL;                  // To hold settlement's player by ID (IL = Intersecntion Location)
        map<pair<int, int>, string> RL;  // To hold road's player by IDs(RL = Road Location)

        // Initialize intersection labels with spaces or player IDs if a settlement/city is present
        for (int i = 1; i <= 54; ++i) 
        {
            if (settlements.find(i) != settlements.end() && !settlements.at(i).empty()) 
            {
                IL[i] = "|S" + to_string(*settlements.at(i).begin()) + "|";  // Assuming only one player can have a settlement at any intersection
            } else if (cities.find(i) != cities.end()) {
                IL[i] = "|C" + to_string(cities.at(i)) + "|";  // Cities map holds player ID directly
            } else {
                IL[i] = "| |";  // Three spaces for empty intersections
            }
        }

        // Initialize all possible road locations with empty braces
        const vector<pair<int, int>> roadPositions = {
            {28, 17}, {28, 39}, {17, 18}, {39, 29}, {18, 8}, {48, 40}, {30, 19}, {9, 1},
            {48, 49}, {40, 41}, {30, 31}, {19, 20}, {9, 10}, {49, 50}, {41, 42}, {31, 32},
            {20, 21}, {10, 11}, {50, 42}, {32, 21}, {11, 3}, {50, 51}, {42, 43}, {32, 33},
            {21, 22}, {11, 12}, {51, 52}, {43, 44}, {33, 34}, {22, 23}, {12, 13}, {52, 44},
            {34, 23}, {13, 5}, {52, 53}, {44, 45}, {34, 35}, {23, 24}, {13, 14}, {53, 45},
            {35, 24}, {14, 6}, {53, 54}, {45, 46}, {35, 36}, {24, 25}, {14, 15}, {54, 46},
            {36, 25}, {15, 7}, {46, 47}, {36, 26}, {25, 16}, {15, 7}, {47, 37}, {26, 16},
            {37, 27}, {26, 16}, {37, 38}, {27, 38}, {20, 10}, {39, 40}, {29, 30}, {18, 19},
            {8, 9}, {41, 31}, {2, 1}, {3, 2}, {43, 33}, {12, 22}, {3, 4}, {12, 13}, {4, 5},
            {5, 6}, {6, 7}, {14, 24}, {36, 37}, {25, 26}, {15, 16}, {26, 27}};

        for (const auto& pos : roadPositions) {
            RL[{pos.first, pos.second}] = "{ }";
            RL[{pos.second, pos.first}] = "{ }";  // Ensure both directions are initialized
        }

        // Update road labels with player IDs if a road is present, for both directions
        for (const auto& [edge, playerID] : roads) {
            int startID = getIntersectionID(edge.getIntersection1());
            int endID = getIntersectionID(edge.getIntersection2());
            RL[{startID, endID}] = "{R" + std::to_string(playerID) + "}";
            RL[{endID, startID}] = "{R" + std::to_string(playerID) + "}";  // Ensure both directions are updated
        }
        
        std::cout << "                                  "<<IL[28]<<"--"<<RL[{17, 28}]<<"--"<<IL[17]<<"                                        " << std::endl;
        std::cout << "                                 /             \\                                        " << std::endl;
        std::cout << "                                "<<RL[{39, 28}]<<"           "<<RL[{17, 18}]<<"                                      " << std::endl;
        std::cout << "                               /        9       \\                                        " << std::endl;
        std::cout << "                    "<<IL[39]<<"--"<<RL[{39, 29}]<<"--"<<IL[29]<<"     Fields     "<<IL[18]<<"--"<<RL[{18, 8}]<<"--"<<IL[8]<<"                           " << std::endl;
        std::cout << "                   /            \\                /            \\                           " << std::endl;
        std::cout << "                 "<<RL[{39, 40}]<<"            "<<RL[{29, 30}]<<"            "<<RL[{18, 19}]<<"            "<<RL[{8, 9}]<<"                        " << std::endl;
        std::cout << "                 /        8       \\           /        12       \\                      " << std::endl;
        std::cout << "     "<<IL[48]<<"--"<<RL[{48, 40}]<<"--"<<IL[40]<<"     Forest    "<<IL[30]<<"--"<<RL[{30, 19}]<<"--"<<IL[19]<<"      Fields      "<<IL[9]<<"--"<<RL[{9, 1}]<<"--"<<IL[1]<<"              " << std::endl;
        std::cout << "     /           \\                /           \\                 /            \\           " << std::endl;
        std::cout << "   "<<RL[{48, 49}]<<"           "<<RL[{40, 41}]<<"            "<<RL[{30, 31}]<<"           "<<RL[{20, 19}]<<"            "<<RL[{10, 9}]<<"            "<<RL[{2, 1}]<<"           " << std::endl;
        std::cout << "   /      5        \\           /       11       \\             /      10        \\           " << std::endl;
        std::cout << " "<<IL[49]<<"   Hills      "<<IL[41]<<"--"<<RL[{41, 31}]<<"--"<<IL[31]<<"     Forest      "<<IL[20]<<"--"<<RL[{20, 10}]<<"--"<<IL[10]<<"    Mountain   "<<IL[2]<<"       " << std::endl;
        std::cout << "  \\               /           \\                 /             \\                /          " << std::endl;
        std::cout << "   "<<RL[{50, 49}]<<"          "<<RL[{42, 41}]<<"          "<<RL[{32, 31}]<<"             "<<RL[{21, 20}]<<"             "<<RL[{11, 10}]<<"             "<<RL[{3, 2}]<<"          " << std::endl;
        std::cout << "     \\           /      3       \\            /       6          \\             /          " << std::endl;
        std::cout << "    "<<IL[50]<<"--"<<RL[{50, 42}]<<"--"<<IL[42]<<"    Mountain    "<<IL[32]<<"--"<<RL[{32, 21}]<<"--"<<IL[21]<<"      Hills        "<<IL[11]<<"--"<<RL[{3, 11}]<<"--"<<IL[3]<<"             " << std::endl;
        std::cout << "    /            \\               /            \\                 /            \\          " << std::endl;
        std::cout << "  "<<RL[{50, 51}]<<"            "<<RL[{43, 42}]<<"            "<<RL[{32, 33}]<<"            "<<RL[{22, 21}]<<"             "<<RL[{12, 11}]<<"           "<<RL[{3, 4}]<<"           " << std::endl;
        std::cout << "  /       9        \\           /                \\             /      2         \\          " << std::endl;
        std::cout << " "<<IL[51]<<"   Fields      "<<IL[43]<<"--"<<RL[{43, 33}]<<"--"<<IL[33]<<"    Desert      "<<IL[22]<<"--"<<RL[{12, 22}]<<"--"<<IL[12]<<"     Pasture    "<<IL[4]<<"      " << std::endl;
        std::cout << "  \\                 /          \\                /            \\                /          " << std::endl;
        std::cout << "   "<<RL[{51, 52}]<<"            "<<RL[{43, 44}]<<"          "<<RL[{33, 34}]<<"           "<<RL[{23, 22}]<<"             "<<RL[{12, 13}]<<"            "<<RL[{4, 5}]<<"          " << std::endl;
        std::cout << "    \\            /       4       \\            /       4        \\            /          " << std::endl;
        std::cout << "     "<<IL[52]<<"--"<<RL[{44, 52}]<<"--"<<IL[44]<<"     Fields    "<<IL[34]<<"--"<<RL[{23, 34}]<<"--"<<IL[23]<<"      Pasture     "<<IL[13]<<"--"<<RL[{5, 13}]<<"--"<<IL[5]<<"             " << std::endl;
        std::cout << "    /            \\               /            \\                 /           \\              " << std::endl;
        std::cout << "  "<<RL[{52, 53}]<<"            "<<RL[{44, 45}]<<"           "<<RL[{34, 35}]<<"             "<<RL[{23, 24}]<<"            "<<RL[{13, 14}]<<"           "<<RL[{5, 6}]<<"           " << std::endl;
        std::cout << "  /      11        \\           /       3         \\            /       9        \\           " << std::endl;
        std::cout << " "<<IL[53]<<"   Pasture    "<<IL[45]<<"--"<<RL[{35, 45}]<<"--"<<IL[35]<<"   Forest       "<<IL[24]<<"--"<<RL[{14, 24}]<<"--"<<IL[14]<<"      Forest    "<<IL[6]<<"      " << std::endl;
        std::cout << "  \\                /           \\                /            \\                  /          " << std::endl;
        std::cout << "  "<<RL[{53, 54}]<<"            "<<RL[{45, 46}]<<"           "<<RL[{35, 36}]<<"            "<<RL[{24, 25}]<<"            "<<RL[{14, 15}]<<"              "<<RL[{6, 7}]<<"          " << std::endl;
        std::cout << "   \\            /      5        \\            /        10       \\              /          " << std::endl;
        std::cout << "    "<<IL[54]<<"--"<<RL[{54, 46}]<<" --"<<IL[46]<<"   Pasture    "<<IL[36]<<"--"<<RL[{25, 36}]<<"--"<<IL[25]<<"      Hills      "<<IL[15]<<"--"<<RL[{7, 15}]<<"--"<<IL[7]<<"             " << std::endl;
        std::cout << "                 \\               /           \\                 /                       " << std::endl;
        std::cout << "                  "<<RL[{46, 47}]<<"          "<<RL[{36, 37}]<<"            "<<RL[{25, 26}]<<"             "<<RL[{15, 16}]<<"                       " << std::endl;
        std::cout << "                   \\           /      8        \\             /                           " << std::endl;
        std::cout << "                    "<<IL[47]<<"--"<<RL[{37, 47}]<<"--"<<IL[37]<<"   Mountain    "<<IL[26]<<"--"<<RL[{16, 26}]<<"--"<<IL[16]<<"                           " << std::endl;
        std::cout << "                               \\                /                                        " << std::endl;
        std::cout << "                                "<<RL[{37, 38}]<<"           "<<RL[{26, 27}]<<"                                        " << std::endl;
        std::cout << "                                  \\            /                                        " << std::endl;
        std::cout << "                                "<<IL[38]<<"--"<<RL[{27, 38}]<<"--"<<IL[27]<<"                                        " << std::endl;

    }
}