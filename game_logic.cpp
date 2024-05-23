#include "game_logic.hpp"

namespace ariel {

    void GameLogic::performTurn(Player* currentPlayer, Board& board, vector<Player*>& players) {
        int diceRoll = rollDice();
        if (diceRoll == 7) {
            for (auto& player : players) {
                if (player->totalResources() > 7) {
                    player->discardHalfResources();
                }
            }
        } else {
            distributeResources(board, players, diceRoll);
        }
        // Development card usage can be handled here if it affects turn logic
    }

    void GameLogic::distributeResources(Board& board, std::vector<Player*>& players, int diceRoll) 
    {
        // Implement the logic to distribute resources to players based on the dice roll and their board positions
    }

    bool GameLogic::checkVictoryConditions(const Player* player) 
    {
        return player->getPoints() >= 10;
    }
}