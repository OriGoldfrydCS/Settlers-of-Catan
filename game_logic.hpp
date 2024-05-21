#ifndef GAME_LOGIC_HPP
#define GAME_LOGIC_HPP

#include "player.hpp"
#include "board.hpp"
#include <vector>
#include <random>

namespace ariel {

class GameLogic {
public:
    // Chooses the starting player index randomly from the number of players.
    static int chooseStartingPlayer(const vector<Player*>& players);

    // Preforms a single player's turn including rolling dice and distributing resources.
    static void performTurn(Player* currentPlayer, Board& board, vector<Player*>& players);

    // Rolls a single six-sided die.
    static int rollDice();

    // Distributes resources to players based on the dice roll and their board positions.
    static void distributeResources(Board& board, vector<Player*>& players, int diceRoll);

    // Checks if the current player has met the victory conditions.
    static bool checkVictoryConditions(const Player* player);
};

}

#endif
