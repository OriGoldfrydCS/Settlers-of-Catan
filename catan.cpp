#include "catan.hpp"
#include <iostream>
#include <stdexcept>

using namespace std;
namespace ariel {


    // Constructor
    Catan::Catan(Player& p1, Player& p2, Player& p3) {
        players.push_back(&p1);
        players.push_back(&p2);
        players.push_back(&p3);
        currentPlayerIndex = 0;
    }

    // In Settlers of Catan, the game board is composed of 19 hexagonal tiles arranged to form a 
    // larger hexagon. Here’s a visual representation of axial coordinates on a hexagonal grid, 
    // as implemented. Each hexagonal represents as (q,r) , where q represents the horizontal position
    // of a hex tile; and r represents the diagonal position of a hex tile from top left to bottom right.
    // Here is an illustration:
    //
    //            (-1,1)   (0,1)   (1,1)
    //        (-2,2)   (-1,2)   (0,2)   (1,2)
    //    (-3,3)   (-2,3)   (-1,3)   (0,3)   (1,3)
    //        (-3,2)   (-2,2)   (-1,2)   (0,2)
    //            (-3,1)   (-2,1)   (-1,1)
    
    void Catan::initializeGame() 
    {
        board = Board();
        currentPlayerIndex = GameLogic::chooseStartingPlayer(players);
        // Implement the logic to initialize the game, set up the board, and choose the starting player
    }

    void Catan::ChooseStartingPlayer() 
    {
        // Implement
    }
    
    void Catan::playGame() 
    {
        while (true) 
        {
            Player* currentPlayer = players[currentPlayerIndex];
            GameLogic::performTurn(currentPlayer, board);
            if (GameLogic::checkVictoryConditions(currentPlayer)) 
            {
                cout << "Player " << currentPlayer->getPoints() << " wins!" << endl;
                break;
            }
            nextTurn();
        }
        // Implement the main game loop, controlling the flow of turns and checking for the game end condition
    }

    void Catan::nextTurn() 
    {
        currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
        // Implement the logic to advance the game to the next player's turn
    }

    void Catan::printGameState() const 
    {
        cout << "Game state:" << endl;
        for (const auto& player : players) 
        {
            player->printPoints();
        }
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