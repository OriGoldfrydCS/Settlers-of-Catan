#ifndef CATAN_HPP
#define CATAN_HPP

#include "player.hpp"
#include "board.hpp"
#include "game_logic.hpp"
#include <vector>

using namespace std;
namespace ariel {
    
    class Catan 
    {
        private:
            vector<Player*> players;
            Board board;
            GameLogic gameLogic;
            size_t currentPlayerIndex;

        public:

        // Constructor that initializes the game with a list of players.
        Catan(const vector<Player*>& players);                              // TO DO: Exception if less than 3 players

        // Initializes the game, setting up the board and choosing the starting player.
        void initializeGame();

        // Manages the main game loop, controlling the flow of turns and checking for the game end condition.
        void playGame();

        // Advances the game to the next player's turn, updating the currentPlayerIndex.
        void nextTurn();

        // Displays the current state of the game, including players' statuses and the board state.
        void printGameState() const;

        // Provides access to the game board, allowing other components of the program to interact with it.
        Board& getBoard();

        // Prints the winner of the game based on current scores.
        void printWinner();

    };
}

#endif