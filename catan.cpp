#include "catan.hpp"
#include <iostream>
#include <stdexcept>

using namespace std;
namespace ariel {

    Catan::Catan(const std::vector<Player*>& players) : players(players), currentPlayerIndex(0) 
    {
        if (players.size() < 3) 
        {
            throw std::invalid_argument("Error: There must be at least 3 players");
        }
        // Implement the logic to initialize the Catan game
    }

    void Catan::initializeGame() 
    {
        board = Board();
        currentPlayerIndex = GameLogic::chooseStartingPlayer(players);
        // Implement the logic to initialize the game, set up the board, and choose the starting player
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