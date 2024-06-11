// Email: origoldbsc@gmail.com

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
            size_t currentPlayerIndex;  // Index to track the current player's turn

            // Related to the the main game loop which controlling the flow of turns (void playGame())
            void handleBuildSettlement(Player* currentPlayer);
            void handleUpgradeToCity(Player* currentPlayer);

            // Related to initializing the game (void initializeGame())
            void ChooseStartingPlayer();

            // Handles the game logic when a player rolls a 7
            void handleSevenRoll();

        public:

            // Constructor that initializes the game with three player references
            Catan(Player& p1, Player& p2, Player& p3);

            // Initializes the game, setting up the board, distribute resources and choosing the starting player
            void initializeGame();

            // Manages the main game loop, controlling the flow of turns and checking for the game end condition
            void playGame();        
            void distributeResources(Player* player);   // TO-DO: change to private method after presentions (should be public for testing purpose)
            void handleBuyDevelopmentCard(Player* currentPlayer);
            void handleBuildRoad(Player* currentPlayer);
            void handleDevelopmentCardUsage(Player* currentPlayer, bool& shouldEndTurn);

            // Advances the game to the next player's turn
            void nextTurn();                            // TO-DO: change to private method after presentions

            // Displays the current state of the game, including players' statuses and the board state
            void printGameState() const;

            // Provides access to the game board and players
            Board& getBoard();
            vector<Player*>& getPlayers();

            // Prints the winner of the game
            void printWinner();

            // Clears the input buffer to prevent input errors during user interaction
            void clearInputBuffer();

            // Initializes the board for testing purposes, without placing any settlements or distributing resources
            void testInitialize();

            // Checks if there is a winner in the game for testing purposes
            void hasWinner();
    };
}

#endif