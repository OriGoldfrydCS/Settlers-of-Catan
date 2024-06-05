#ifndef CATAN_HPP
#define CATAN_HPP

#include "player.hpp"
#include "board.hpp"
#include <vector>

using namespace std;
namespace ariel {
    
    class Catan 
    {
        private:
        
            vector<Player*> players;    // Stores pointers to the players participating in the game
            Board board;                // Represents the game board containing all game elements
            size_t currentPlayerIndex;  // Index to track the current player's turn

        public:

        // Constructor that initializes the game with three player references
        Catan(Player& p1, Player& p2, Player& p3);

        // Initializes the game, setting up the board, distribute resources and choosing the starting player
        void initializeGame();
        void distributeResources(Player* player);
        void ChooseStartingPlayer();

        // Manages the main game loop, controlling the flow of turns and checking for the game end condition
        void playGame();        
        void handleBuildRoad(Player* currentPlayer);
        void handleBuildSettlement(Player* currentPlayer);
        void handleUpgradeToCity(Player* currentPlayer);
        void handleBuyDevelopmentCard(Player* currentPlayer);
        void handleDevelopmentCardUsage(Player* currentPlayer, bool& shouldEndTurn);

        // Advances the game to the next player's turn
        void nextTurn() ;

        // Handles the game logic when a player rolls a 7
        void handleSevenRoll();

        // Displays the current state of the game, including players' statuses and the board state
        void printGameState() const;

        // Provides access to the game board
        Board& getBoard();

        // Prints the winner of the game
        void printWinner();

        // Clears the input buffer to prevent input errors during user interaction
        void clearInputBuffer();

        // Initializes the board for testing purposes, without placing any settlements or distributing resources
        void testInitialize();
    };
}

#endif