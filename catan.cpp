// Email: origoldbsc@gmail.com

#include "catan.hpp"
#include "intersection.hpp"
#include "edge.hpp"
#include <iostream>
#include <stdexcept>
#include <algorithm>    
#include <vector>       
#include <utility>      

using namespace std;
namespace ariel {

     /**
     * @brief Constructor that initializes the game with three players.
     * @param p1 Reference to the first player.
     * @param p2 Reference to the second player.
     * @param p3 Reference to the third player.
     */
    Catan::Catan(Player& p1, Player& p2, Player& p3) : players({&p1, &p2, &p3}), currentPlayerIndex(0){}


    /**
     * @brief Sets up the game by configuring tiles, linking them with intersections, and distributing initial resources.
     */
    void Catan::initializeGame() 
    {   
        cout << "Processing initialization due to Catan beginner setup...\n" << endl;

        Board& board = Board::getInstance();
        board.setupTiles();  
        board.linkTilesAndIntersections();

        // Setup each player's initial resources and settlements
        players[0]->placeInitialSettlement(41, board);     // Intersection 41 touches Wood(8), Ore(3), Brick(5)
        players[0]->placeInitialSettlement(45, board);     // Intersection 45 touches Grain(4), Wool(5), Wool(11)
        players[1]->placeInitialSettlement(14, board);     // Road between Wool(4) and Wood(9)
        players[1]->placeInitialSettlement(43, board);     // Road between Grain(4) and Grain(6)
        players[2]->placeInitialSettlement(20, board);     // Road between Grain(12) and Wood(11)
        players[2]->placeInitialSettlement(36, board);     // Road between Wood(3) and Ore(8)

        // Distribute resources after all settlements are placed
        for (auto* player : players) 
        {
            distributeResources(player);
        }
        
        // Place initial roads
        players[0]->placeInitialRoad(Edge(Intersection::getIntersection(41), Intersection::getIntersection(42)), board);   // Road between Ore(3) and Brick(5)
        players[0]->placeInitialRoad(Edge(Intersection::getIntersection(35), Intersection::getIntersection(45)), board);   // Road between Grain(4) and Wool(5)
        players[1]->placeInitialRoad(Edge(Intersection::getIntersection(13), Intersection::getIntersection(14)), board);   // Road between Wool(4) and Wood(9)
        players[1]->placeInitialRoad(Edge(Intersection::getIntersection(43), Intersection::getIntersection(44)), board);   // Road between Grain(4) and Grain(6)
        players[2]->placeInitialRoad(Edge(Intersection::getIntersection(19), Intersection::getIntersection(20)), board);     // Road between Grain(12) and Wood(11)
        players[2]->placeInitialRoad(Edge(Intersection::getIntersection(25), Intersection::getIntersection(36)), board);     // Road between Wood(3) and Ore(8)
        
        cout << "\nInitial resources were distributed to the players." << endl;
        cout << "Settlements & roads places by the game's instructions for beginners.\n" << endl;

        ChooseStartingPlayer();
    }


    /**
     * @brief Distributes resources to a player based on the locations of their settlements.
     * @param player Pointer to the player receiving resources.
     */
    void Catan::distributeResources(Player* player) 
    {
        Board& board = Board::getInstance();
        set<ResourceType> uniqueResources;
        cout << "Player " << player->getName() << " has settlements at: ";
        for (int settlement : player->getSettlements()) 
        {
            cout << settlement << " ";
            auto resources = board.getResourceTypesAroundIntersection(settlement);
            uniqueResources.insert(resources.begin(), resources.end());
        }
        cout << endl;

        // Add one resource card of each type gathered from around the settlements.
        for (auto resource : uniqueResources) 
        {
            player->addResource(resource, 1);       // Add one resource card of each type
        }
    }


    /**
     * @brief Determines the starting player based on dice rolls.
     */
    void Catan::ChooseStartingPlayer() 
    {
        cout << "Roll the dice to determine the player's order: " << endl;
        vector<pair<int, Player*>> diceRolls;

        // Each player rolls a die and their result is stored
        for (auto* player : players) 
        {
            int roll = player->rollDice();
            diceRolls.push_back({roll, player});
            cout << "Player " << player->getName() << " rolls a " << roll << "." << endl;
        }

        // Sort players based on their dice rolls in descending order
        sort(diceRolls.begin(), diceRolls.end(), [](const pair<int, Player*>& a, const pair<int, Player*>& b) 
        {
            return a.first > b.first;  
        });

        // Update the players vector to privde the new order
        for (size_t i = 0; i < diceRolls.size(); ++i) 
        {
            players[i] = diceRolls[i].second;
        }

        currentPlayerIndex = 0;     // The first player in the sorted list starts
        cout << "\nPlayer " << players[currentPlayerIndex]->getName() << " starts the game!" << endl;
        cout << "\nLet's start..." << endl;

    }

    /**
     * @brief Main game loop that controls the flow of the game until a player wins.
     */
    void Catan::playGame() 
    {
        Board& board = Board::getInstance();
        bool gameRunning = true;
        while (gameRunning) 
        {   
            // Iterate through each player for their turn.
            for (Player* currentPlayer : players) 
            {
                cout << "\nIt's " << currentPlayer->getName() << "'s turn." << endl;
                
                int preChoice = 0;
                while (preChoice != 1 && preChoice != 2) 
                {
                    cout << "Do you want to (1) Roll Dice or (2) Use Development Card? ";
                    cin >> preChoice;
                    if (cin.fail() || (preChoice != 1 && preChoice != 2)) 
                    {
                        cout << "\nERROR: Invalid input. Please enter '1' for Roll Dice or '2' for Use Development Card!\n\n";
                        cin.clear();                                            // Clear error flag
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');    // Ignore incorrect input
                    }
                }

                if (preChoice == 2) 
                {
                    bool shouldEndTurn = false;
                    handleDevelopmentCardUsage(currentPlayer, shouldEndTurn);
                    if (shouldEndTurn) 
                    {
                        currentPlayer->endTurn();
                        nextTurn();
                        continue;       // Skip to next player immediately
                    }
                }
             
                else 
                {
                    // Proceed with dice roll
                    int dice1 = Player::rollDice();
                    int dice2 = Player::rollDice();
                    int total = dice1 + dice2;
                    cout << "\nPlayer " << currentPlayer->getName() << " rolls " << dice1 << " + " << dice2 << " = " << total << "." << endl;

                    if (total == 7) 
                    {
                        handleSevenRoll();
                    } 
                    else 
                    {
                        board.distributeResourcesBasedOnDiceRoll(total, players);
                        cout << "\nSTATUS: Resources distributed based on dice roll." << endl;
                    }
                }

                cout << "\nProceeding to action selection..." << endl;

                // Action selection
                bool endTurn = false;
                while (!endTurn) 
                {
                    int choice = -1; // Initialize outside valid range
                    while (choice < 0 || choice > 8) 
                    {
                        cout << "\nChoose an action:\n";
                        cout << "0. View game state\n";
                        cout << "1. View your full player details\n";
                        cout << "2. Trade resources or cards\n";
                        cout << "3. Build a road\n";
                        cout << "4. Build a settlement\n";
                        cout << "5. Upgrade to city\n";
                        cout << "6. Buy development card\n";
                        cout << "7. Use development card\n";
                        cout << "8. End turn\n";
                        cout << "Enter your choice: ";
                        cin >> choice;
                        clearInputBuffer();  // Clear buffer after reading

                        if (choice < 0 || choice > 8) 
                        {
                            cout << "\nERROR: Invalid choice, please choose a number between 0 and 8!\n";
                        }
                    }

                    switch(choice) 
                    {
                        case 0:
                            printGameState();
                            break;
                        case 1:
                            cout << currentPlayer->printPlayer();
                            break;
                        case 2:
                            currentPlayer->trade(players);
                            break;
                        case 3:
                            handleBuildRoad(currentPlayer);
                            // endTurn = true;
                            break;
                        case 4:
                            handleBuildSettlement(currentPlayer);
                            // endTurn = true;
                            break;
                        case 5:
                            handleUpgradeToCity(currentPlayer);
                            // endTurn = true;
                            break;
                        case 6: 
                        {
                            handleBuyDevelopmentCard(currentPlayer);
                            // endTurn = true;
                            break;
                        }

                        case 7:
                        {
                            bool shouldEndTurn = false;
                            handleDevelopmentCardUsage(currentPlayer, shouldEndTurn);
                            if (shouldEndTurn) 
                            {
                                // cout << "Proceeding to additional actions after using the card..." << endl;
                                endTurn = true; // End turn if the card used should end the turn
                            }
                            break;
                        }
                        case 8:
                            currentPlayer->endTurn();
                            nextTurn();
                            endTurn = true;
                            // cout << "Turn ended for " << currentPlayer->getName() << "." << endl;
                            break;
                        default:
                            cout << "Invalid choice, please choose again.\n";
                            break;

                    }
                }


                // Check victory conditions
                if (currentPlayer->getPoints() >= 10) 
                {
                    cout << "Player " << currentPlayer->getName() << " wins with " << currentPlayer->getPoints() << " points!" << endl;
                    gameRunning = false;
                    break;
                }
            }
        }
    }

    /**
     * @brief Handles the construction of a road between two intersections.
     * @param currentPlayer Pointer to the current player attempting to build a road.
     */
    void Catan::handleBuildRoad(Player* currentPlayer) 
    {
        Board& board = Board::getInstance();

        cout << "Enter the intersection IDs to place a road (e.g., 4 5): ";
        int id1, id2;
        cin >> id1 >> id2;
        try {
            Edge edge(Intersection::getIntersection(id1), Intersection::getIntersection(id2));
            if (currentPlayer->canBuild("road") && board.canPlaceRoad(edge, currentPlayer->getId())) 
            {
                currentPlayer->buildRoad(edge, board);
                cout << "\nRoad successfully built for "  << currentPlayer->getName() << " between " << id1 << " and " << id2 << "." << endl;
            } 
            else 
            {
                cout << "Failed to build road. Check if the road is valid or if you have enough resources." << endl;
            }
        } catch (const exception& e) {
            cout << "Invalid intersection IDs provided. Please try again." << endl;
        }
    }


    /**
     * @brief Handles the construction of a settlement at a specified intersection.
     * @param currentPlayer Pointer to the current player attempting to build a settlement.
     */
    void Catan::handleBuildSettlement(Player* currentPlayer) 
    {
        Board& board = Board::getInstance();

        cout << "Enter the intersection ID to place a settlement: ";
        int intersectionID;
        cin >> intersectionID;
        if (currentPlayer->canBuild("settlement") && board.canPlaceSettlement(intersectionID, currentPlayer->getId())) 
        {
            currentPlayer->buildSettlement(intersectionID, board);
            cout << "Settlement successfully built at intersection " << intersectionID << "." << endl;
        } 
        else 
        {
            cout << "Failed to build settlement. Check if the location is valid or if you have enough resources." << endl;
        }
    }


    /**
     * @brief Handles the upgrade of a settlement to a city at a specified intersection.
     * @param currentPlayer Pointer to the current player attempting to upgrade a settlement to a city.
     */
    void Catan::handleUpgradeToCity(Player* currentPlayer) 
    {
        Board& board = Board::getInstance();

        cout << "Enter the intersection ID to upgrade to a city: ";
        int intersectionID;
        cin >> intersectionID;
        if (currentPlayer->canBuild("city") && board.canUpgradeSettlementToCity(intersectionID, currentPlayer->getId())) 
        {
            currentPlayer->upgradeToCity(intersectionID, board);
            cout << "Settlement at intersection " << intersectionID << " has been upgraded to a city." << endl;
        } 
        else 
        {
            cout << "Failed to upgrade to a city. Ensure there is a settlement at the location and you have sufficient resources." << endl;
        }
    }

    /**
     * @brief Manages the purchase of a development card by the current player.
     * This version selects a random card type to be purchased.
     * @param currentPlayer Pointer to the player attempting to buy a development card.
     */
    void Catan::handleBuyDevelopmentCard(Player* currentPlayer)
    {
        CardPurchaseError result = currentPlayer->buyDevelopmentCard(players);
        switch(result) 
        {
            case CardPurchaseError::Success:
                // cout << "\nSTATUS: Development card purchased successfully!\n";     // Printing for debugging
                break;
            case CardPurchaseError::InsufficientResources:
                cout << "\nSTATUS: Not enough resources to buy a development card!\n";
                break;
            case CardPurchaseError::CardUnavailable:
                cout << "\nSTATUS: No development cards available!\n";
                break;
            default:
                cout << "\nSTATUS: An unexpected error occurred during card purchase.\n";
                break;
        }
    }

    // /**
    //  * @brief Manages the purchase of a development card by the current player.
    //  * @param currentPlayer Pointer to the player attempting to buy a development card.
    //  */
    // void Catan::handleBuyDevelopmentCard(Player* currentPlayer)
    // {
    //     cout << "Select the type of Development Card to buy:\n";
    //     cout << "1. Knight\n";
    //     cout << "2. Victory Point\n";
    //     cout << "3. Promotion\n";
    //     cout << "Enter your choice: ";
    //     int cardChoice;
    //     cin >> cardChoice;
    //     DevCardType cardType;

    //     switch(cardChoice)
    //     {
    //         case 1:
    //             cardType = DevCardType::KNIGHT;
    //             break;
    //         case 2:
    //             cardType = DevCardType::VICTORY_POINT;
    //             break;
    //         case 3:
    //             cardType = DevCardType::PROMOTION;
    //             break;
    //         default:
    //             cout << "\nSTATUS: Invalid card type selected.\n";
    //             return;
    //     }

    //     CardPurchaseError result = currentPlayer->buyDevelopmentCard(cardType, players);
    //     switch(result) 
    //     {
    //         case CardPurchaseError::Success:
    //             cout << "\nSTATUS: Development card purchased successfully!\n";
    //             break;
    //         case CardPurchaseError::InsufficientResources:
    //             cout << "\nSTATUS: Not enough resources to buy a development card!\n";
    //             break;
    //         case CardPurchaseError::CardUnavailable:
    //             cout << "\nSTATUS: The selected card is currently unavailable!\n";
    //             break;
    //         default:
    //             cout << "\nSTATUS: An unexpected error occurred...\n";
    //             break;
    //     }
    // }


    /**
     * @brief Manages the usage of a development card by the current player.
     * @param currentPlayer Pointer to the player using the development card.
     * @param shouldEndTurn Reference to a boolean indicating if the turn should end after using the card.
     */
    void Catan::handleDevelopmentCardUsage(Player* currentPlayer, bool& shouldEndTurn) 
    {
        Board& board = Board::getInstance();

        // Assume function that manages the choice and use of a development card
        cout << "\nSelect the type of Development Card to use:\n1. Victory Point\n2. Promotion\nEnter your choice: ";
        int devCardChoice;
        cin >> devCardChoice;
        DevCardType cardType;
        switch(devCardChoice) 
        {
            // case 1:
            //     cardType = DevCardType::KNIGHT;
            //     break;
            case 1:
                cardType = DevCardType::VICTORY_POINT;
                break;
            case 2:
                cardType = DevCardType::PROMOTION;
                break;
            default:
                cout << "\nSTATUS:Invalid card type selected!\n";
                break;
        }
        
        CardUseError useResult = currentPlayer->useDevelopmentCard(cardType, players, board, shouldEndTurn);
        
        if (useResult == CardUseError::Success) 
        {
            cout << "\nSTATUS: Development card used successfully!" << endl;  // Print for debugging
            shouldEndTurn = true;
        } 
        else if (useResult == CardUseError::InsufficientCards) 
        {
            cout << "\nSTATUS: You do not have enough of this card to use!" << endl;
        } 
        else 
        {
            cout << "\nSTATUS: Invalid card type or other error." << endl;
        }
    }


    /**
     * @brief Handles the event when a player rolls a seven, enforcing the discarding of resources.
     */
    void Catan::handleSevenRoll() 
    {
        cout << "\nA 7 was rolled. Players with more than 7 resources must discard half of them." << endl;
        size_t currentPlayerPosition = currentPlayerIndex; // Remember the current player index
        do {
            Player* currentPlayer = players[currentPlayerIndex];
            int totalResources = currentPlayer->countTotalResources();
            if (totalResources > 7) 
            {
                int toDiscard = totalResources / 2; // Calculate half to discard, rounded down
                currentPlayer->discardResources(toDiscard);
            }
            nextTurn(); // Proceed to next player in a circular manner
        } while (currentPlayerIndex != currentPlayerPosition); // Go until we reach the starting player again

        cout << "\nIt is now " << players[currentPlayerIndex]->getName() << "'s turn to continue their turn." << endl;
    }


    /**
     * @brief Clears the input buffer to handle potential input errors.
     */
    void Catan::clearInputBuffer() 
    {
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }


    /**
     * @brief Advances the game to the next player's turn.
     */
    void Catan::nextTurn() 
    {
        currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
        cout << "It is now " << players[currentPlayerIndex]->getName() << "'s turn." << endl;
    }


    /**
     * @brief Prints the current state of the game, including player points and board status.
     */
    void Catan::printGameState() const 
    {
        Board& board = Board::getInstance();

        cout << "\n";
        cout << "**********************************************************************\n";
        cout << "*                                                                    *\n";
        cout << "*                         🌟 GAME STATE 🌟                           *\n";
        cout << "*                                                                    *\n";
        cout << "**********************************************************************\n";

        for (const auto& player : players) 
        {
            player->printPoints();  
        }

        cout << "**********************************************************************\n";
        cout << "*                                                                    *\n";
        cout << "*                       🌍  BOARD STATUS  🌍                         *\n";
        cout << "*                                                                    *\n";
        cout << "**********************************************************************\n";

        cout << "\n";

        board.printGameBoard();
    }


    /**
     * @brief Returns a reference to the game board.
     * @return Reference to the board used in the game.
     */
    Board& Catan::getBoard() 
    {
        Board& board = Board::getInstance();
        return board;
    }


    /**
     * @brief Determines and prints the winner of the game based on the points accumulated.
     * This function iterates through all players to find the one with the highest points and declares them as the winner.
     */
    void Catan::printWinner() 
    {
        Player* winner = nullptr;
        int maxPoints = 0;
        for (const auto& player : players) 
        {
            if (player->getPoints() > maxPoints) 
            {
                maxPoints = player->getPoints();
                winner = player;
            }
        }
        if (winner) 
        {
            cout << "Winner: " << winner->getPoints() << " points" << endl;
        }
    }

    /**
     * @brief Returns a vector of pointers to all the players in the game.
     * @return A vector of pointers to the players.
     */
    vector<Player*>& Catan::getPlayers() 
    {
        return players;
    }

    /**
     * @brief Initializes the game board for testing purposes without placing any settlements or distributing resources.
     * 
     * This method is intended for testing configurations where you need to verify board setup independent of game logic.
     */
    void Catan::testInitialize() 
    {
        Board& board = Board::getInstance();
        board.setupTiles();
        board.linkTilesAndIntersections();
    }

    /**
     * @brief Checks if there is a winner in the game. If there is a winner, it prints the winner's details. 
     * If no player has reached 10 points yet, it prints the leading player and the points needed to win.
     */
    void Catan::hasWinner() 
    {
        Player* leader = nullptr;
        int maxPoints = 0;

        // Determine the player with the highest points
        for (auto* player : players) {
            if (player->getPoints() > maxPoints) {
                maxPoints = player->getPoints();
                leader = player;
            }
        }

        // Check if the leading player has 10 or more points to declare a winner
        if (leader && maxPoints >= 10) 
        {
            cout << "\nWe have a winner! " << leader->getName() << " wins the game with " << maxPoints << " points!" << endl;
        } 
        else 
        {
            cout << "\nNo winner yet. Reach 10 points to win the game. ";
            if (leader) 
            {
                cout << leader->getName() << " is leading with " << maxPoints << " points." << endl;
            } 
            else 
            {
                cout << "\nNo players have points yet." << endl;
            }
        }
    }
}