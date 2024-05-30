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


    // Constructor
    Catan::Catan(Player& p1, Player& p2, Player& p3) : players({&p1, &p2, &p3}), currentPlayerIndex(0) 
    {
        initializeGame();
    }

    void Catan::initializeGame() 
    {
        board.setupTiles();  
        board.linkTilesAndIntersections();

        
         // Setup each player's initial resources and settlements
        players[0]->placeInitialSettlement(41, board);     // Intersection 41 touches Wood(8), Ore(3), Brick(5)
        players[0]->placeInitialSettlement(45, board);     // Intersection 45 touches Grain(4), Wool(5), Wool(11)
        players[1]->placeInitialSettlement(14, board);     // Road between Wool(4) and Wood(9)
        players[1]->placeInitialSettlement(43, board);     // Road between Grain(4) and Grain(6)
        players[2]->placeInitialSettlement(20, board);     // Road between Grain(12) and Wood(11)
        players[2]->placeInitialSettlement(36, board);     // Road between Wood(3) and Ore(8)

        // Now distribute resources after all settlements are placed
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
        
        cout << "Initial resources were distributed to the players" << endl;
        cout << "Settlements & roads places by the game's instructions for beginners" << endl;

    }


   
    // void Catan::placeInitialSettlementAndResources(int intersectionID, Player* player) {
    //     player->placeSettlement(intersectionID, board);

    //     // Check if settlement was successfully placed
    //     if (board.hasSettlement(intersectionID)) 
    //     {  
    //         distributeInitialResources(intersectionID, player);
    //     }
    // }

    void Catan::distributeResources(Player* player) 
    {
        set<ResourceType> uniqueResources;
        for (int settlement : player->getSettlements()) 
        {
            auto resources = board.getResourceTypesAroundIntersection(settlement);
            uniqueResources.insert(resources.begin(), resources.end());
        }

        for (auto resource : uniqueResources) 
        {
            player->addResource(resource, 1);       // Add one resource card of each type
        }
    }

    void Catan::ChooseStartingPlayer() 
    {
        cout << "Roll the dice to determine the player's order: " << endl;
        vector<pair<int, Player*>> diceRolls;

        // Each player rolls a die and their result is stored with their reference
        for (auto* player : players) 
        {
            int roll = player->rollDice();
            diceRolls.push_back({roll, player});
            cout << "Player " << player->getName() << " rolls a " << roll << "." << endl;
        }

        // Sort players based on their dice rolls in descending order
        sort(diceRolls.begin(), diceRolls.end(), [](const pair<int, Player*>& a, const pair<int, Player*>& b) 
        {
            return a.first > b.first;  // Sort by the first element of the pair (dice roll) in descending order
        });

        // Update the players vector to reflect the new order
        for (size_t i = 0; i < diceRolls.size(); ++i) 
        {
            players[i] = diceRolls[i].second;
        }

        // Set the current player index to 0 since the first player in the vector is the one who rolled the highest
        currentPlayerIndex = 0;

        cout << "Player " << players[currentPlayerIndex]->getName() << " starts the game!" << endl;
        cout << "Let's start..." << endl;

    }

    void Catan::playGame() 
    {
        bool gameRunning = true;
        while (gameRunning) 
        {
            for (Player* currentPlayer : players) 
            {
                cout << "\nIt's " << currentPlayer->getName() << "'s turn." << endl;
                cout << "Do you want to (1) Roll Dice or (2) Use Development Card? ";

                int preChoice;
                cin >> preChoice;
                if (preChoice == 2) 
                {
                    bool shouldEndTurn = false;
                    handleDevelopmentCardUsage(currentPlayer, shouldEndTurn);
                    if (shouldEndTurn) {
                        currentPlayer->endTurn();
                        nextTurn();
                        continue; // Skip to next player immediately
                    }
                }
                else {
                    // Proceed with dice roll
                    int dice1 = Player::rollDice();
                    int dice2 = Player::rollDice();
                    int total = dice1 + dice2;
                    cout << "Player " << currentPlayer->getName() << " rolls " << dice1 << " + " << dice2 << " = " << total << "." << endl;

                    if (total == 7) {
                        handleSevenRoll();
                    } else {
                        board.distributeResourcesBasedOnDiceRoll(total, players);
                        cout << "Resources distributed based on dice roll." << endl;
                    }
                }

                cout << "Proceeding to action selection..." << endl;

                // Action selection
                bool endTurn = false;
                while (!endTurn) {
                    cout << "\nChoose an action:\n";
                    cout << "0. View game state\n";
                    cout << "1. View your full player details\n";
                    cout << "2. Trade resources\n";
                    cout << "3. Build a road\n";
                    cout << "4. Build a settlement\n";
                    cout << "5. Upgrade to city\n";
                    cout << "6. Buy development card\n";
                    cout << "7. Use development card\n";
                    cout << "8. End turn\n";
                    cout << "Enter your choice: ";
                    int choice;
                    cin >> choice;
                    clearInputBuffer(); // Clear the buffer after reading an integer

                    switch(choice) {
                        case 0:
                            printGameState();
                            break;
                        case 1:
                            cout << currentPlayer->printPlayer();
                            break;
                        case 2:
                            currentPlayer->trade(players);
                            cout << "Trading resources...\n";
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
                            if (shouldEndTurn) {
                                cout << "Proceeding to additional actions after using the card..." << endl;
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

                if (currentPlayer->getPoints() >= 10) {
                    cout << "Player " << currentPlayer->getName() << " wins with " << currentPlayer->getPoints() << " points!" << endl;
                    gameRunning = false;
                    break;
                }
            }
        }
    }

    void Catan::handleBuildRoad(Player* currentPlayer) {
        cout << "Enter the intersection IDs to place a road (e.g., 4 5): ";
        int id1, id2;
        cin >> id1 >> id2;
        try {
            Edge edge(Intersection::getIntersection(id1), Intersection::getIntersection(id2));
            if (currentPlayer->canBuild("road") && board.canPlaceRoad(edge, currentPlayer->getId())) {
                currentPlayer->buildRoad(edge, board);
                cout << "Road successfully built between " << id1 << " and " << id2 << "." << endl;
            } else {
                cout << "Failed to build road. Check if the road is valid or if you have enough resources." << endl;
            }
        } catch (const std::exception& e) {
            cout << "Invalid intersection IDs provided. Please try again." << endl;
        }
    }

    void Catan::handleBuildSettlement(Player* currentPlayer) {
        cout << "Enter the intersection ID to place a settlement: ";
        int intersectionID;
        cin >> intersectionID;
        if (currentPlayer->canBuild("settlement") && board.canPlaceSettlement(intersectionID, currentPlayer->getId())) {
            currentPlayer->buildSettlement(intersectionID, board);
            cout << "Settlement successfully built at intersection " << intersectionID << "." << endl;
        } else {
            cout << "Failed to build settlement. Check if the location is valid or if you have enough resources." << endl;
        }
    }

    void Catan::handleUpgradeToCity(Player* currentPlayer) {
        cout << "Enter the intersection ID to upgrade to a city: ";
        int intersectionID;
        cin >> intersectionID;
        if (currentPlayer->canBuild("city") && board.canUpgradeSettlementToCity(intersectionID, currentPlayer->getId())) {
            currentPlayer->upgradeToCity(intersectionID, board);
            cout << "Settlement at intersection " << intersectionID << " has been upgraded to a city." << endl;
        } else {
            cout << "Failed to upgrade to a city. Ensure there is a settlement at the location and you have sufficient resources." << endl;
        }
    }

    void Catan::handleBuyDevelopmentCard(Player* currentPlayer) {
        cout << "Select the type of Development Card to buy:\n";
        cout << "1. Knight\n";
        cout << "2. Victory Point\n";
        cout << "3. Promotion\n";
        cout << "Enter your choice: ";
        int cardChoice;
        cin >> cardChoice;
        DevCardType cardType;

        switch(cardChoice) {
            case 1:
                cardType = DevCardType::KNIGHT;
                break;
            case 2:
                cardType = DevCardType::VICTORY_POINT;
                break;
            case 3:
                cardType = DevCardType::PROMOTION;
                break;
            default:
                cout << "Invalid card type selected.\n";
                return;
        }

        CardPurchaseError result = currentPlayer->buyDevelopmentCard(cardType);
        switch(result) {
            case CardPurchaseError::Success:
                cout << "Development card purchased successfully.\n";
                break;
            case CardPurchaseError::InsufficientResources:
                cout << "Not enough resources to buy a development card.\n";
                break;
            case CardPurchaseError::CardUnavailable:
                cout << "The selected card is currently unavailable.\n";
                break;
            default:
                cout << "An unexpected error occurred.\n";
                break;
        }
    }

    void Catan::handleDevelopmentCardUsage(Player* currentPlayer, bool& shouldEndTurn) {
        // Assume function that manages the choice and use of a development card
        cout << "Select the type of Development Card to use:\n1. Knight\n2. Victory Point\n3. Promotion\nEnter your choice: ";
        int devCardChoice;
        cin >> devCardChoice;
        DevCardType cardType;
        switch(devCardChoice) 
        {
            case 1:
                cardType = DevCardType::KNIGHT;
                break;
            case 2:
                cardType = DevCardType::VICTORY_POINT;
                break;
            case 3:
                cardType = DevCardType::PROMOTION;
                break;
            default:
                cout << "Invalid card type selected.\n";
                break;
        }
        CardUseError useResult = currentPlayer->useDevelopmentCard(cardType, players, board, shouldEndTurn);
        if (useResult == CardUseError::Success) {
            cout << "Development card used successfully." << endl;
            shouldEndTurn = true;
            } else if (useResult == CardUseError::InsufficientCards) {
                cout << "You do not have enough of this card to use." << endl;
            } else {
                cout << "Invalid card type or other error." << endl;
        }
    }

    void Catan::handleSevenRoll() {
        cout << "A 7 was rolled. Players with more than 7 resources must discard half of them." << endl;
        size_t currentPlayerPosition = currentPlayerIndex; // Remember the current player index
        do {
            Player* currentPlayer = players[currentPlayerIndex];
            int totalResources = currentPlayer->countTotalResources();
            if (totalResources > 7) {
                int toDiscard = totalResources / 2; // Calculate half to discard, rounded down
                currentPlayer->discardResources(toDiscard);
            }
            nextTurn(); // Proceed to next player in a circular manner
        } while (currentPlayerIndex != currentPlayerPosition); // Go until we reach the starting player again

        cout << "It is now " << players[currentPlayerIndex]->getName() << "'s turn to continue their turn." << endl;
    }



    void Catan::clearInputBuffer() {
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }



    void Catan::nextTurn() 
    {
        currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
        cout << "It is now " << players[currentPlayerIndex]->getName() << "'s turn." << endl;
    }

    // void Catan::playGame() 
    // {
    //     while (true) 
    //     {
    //         Player* currentPlayer = players[currentPlayerIndex];
    //         GameLogic::performTurn(currentPlayer, board);
    //         if (GameLogic::checkVictoryConditions(currentPlayer)) 
    //         {
    //             cout << "Player " << currentPlayer->getPoints() << " wins!" << endl;
    //             break;
    //         }
    //         nextTurn();
    //     }
    //     // Implement the main game loop, controlling the flow of turns and checking for the game end condition
    // }

    // void Catan::nextTurn() 
    // {
    //     currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
    //     // Implement the logic to advance the game to the next player's turn
    // }

    void Catan::printGameState() const 
    {
        cout << "-------------------------------------------------------" << endl;
        cout << "|                     GAME STATE                      |" << endl;
        cout << "-------------------------------------------------------" << endl;

        for (const auto& player : players) 
        {
            player->printPoints();
        }
        cout << "*******************************************************" << endl;
        cout << "                     Board Status                      " << endl;
        board.printBoard();
        // Implement the logic to display the current state of the game
    }

    Board& Catan::getBoard() 
    {
        return this->board;
    }

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
        // Implement the logic to print the winner of the game
    }
}