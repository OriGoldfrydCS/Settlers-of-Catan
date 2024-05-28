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
        players[0]->placeSettlement(41, board);     // Intersection 41 touches Wood(8), Ore(3), Brick(5)
        players[0]->placeSettlement(45, board);     // Intersection 45 touches Grain(4), Wool(5), Wool(11)
        players[1]->placeSettlement(14, board);     // Road between Wool(4) and Wood(9)
        players[1]->placeSettlement(43, board);     // Road between Grain(4) and Grain(6)
        players[2]->placeSettlement(20, board);     // Road between Grain(12) and Wood(11)
        players[2]->placeSettlement(36, board);     // Road between Wood(3) and Ore(8)

        // Now distribute resources after all settlements are placed
        for (auto* player : players) 
        {
            distributeResources(player);
        }
        
        // Place initial roads
        players[0]->placeRoad(Edge(Intersection::getIntersection(41), Intersection::getIntersection(42)), board);   // Road between Ore(3) and Brick(5)
        players[0]->placeRoad(Edge(Intersection::getIntersection(35), Intersection::getIntersection(45)), board);   // Road between Grain(4) and Wool(5)
        players[1]->placeRoad(Edge(Intersection::getIntersection(13), Intersection::getIntersection(14)), board);   // Road between Wool(4) and Wood(9)
        players[1]->placeRoad(Edge(Intersection::getIntersection(43), Intersection::getIntersection(44)), board);   // Road between Grain(4) and Grain(6)
        players[2]->placeRoad(Edge(Intersection::getIntersection(19), Intersection::getIntersection(20)), board);     // Road between Grain(12) and Wood(11)
        players[2]->placeRoad(Edge(Intersection::getIntersection(25), Intersection::getIntersection(36)), board);     // Road between Wood(3) and Ore(8)
        
        cout << "Settlements and roads are places according to the game's instructions for beginner " << endl;

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
        cout << "Now roll the dice to determine the player's order: " << endl;
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
                cout << "Press 'r' when you are ready to roll the dice." << endl;
                char ready;
                cin >> ready;
                if (ready != 'r' && ready != 'R') {
                    continue; // Skip to the next player if not ready
                }

                bool validRoll = false;
                int total;
                while (!validRoll) {
                    int dice1 = Player::rollDice();
                    int dice2 = Player::rollDice();
                    total = dice1 + dice2;
                    cout << "Player " << currentPlayer->getName() << " rolls a " << dice1 << " and a " << dice2 << " = " << total << "." << endl;

                    if (total == 7) {
                        cout << "Rolled a 7. No resources this turn, roll again." << endl;
                    } else {
                        validRoll = true;
                    }
                }

                if (validRoll && total != 7) {
                    board.distributeResourcesBasedOnDiceRoll(total, currentPlayer);
                }

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
                    cout << "7. End turn\n";
                    cout << "Enter your choice: ";
                    int choice;
                    cin >> choice;

                    switch(choice) {
                        case 0:
                            printGameState();
                            break;
                        case 1:
                            cout << currentPlayer->printPlayer();
                            break;
                        case 2:
                            cout << "Trading resources...\n";
                            break;
                        case 3:
                            cout << "Building a road...\n";
                            break;
                        case 4:
                            cout << "Building a settlement...\n";
                            break;
                        case 5:
                            cout << "Upgrading to a city...\n";
                            break;
                        case 6:
                            cout << "Buying a development card...\n";
                            break;
                        case 7:
                            currentPlayer->endTurn();
                            nextTurn();
                            endTurn = true;
                            cout << "Turn ended for " << currentPlayer->getName() << "." << endl;
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