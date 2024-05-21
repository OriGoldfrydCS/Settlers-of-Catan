#include "game_logic.hpp"

namespace ariel {
    
    int GameLogic::chooseStartingPlayer(const std::vector<Player*>& players) 
    {
        srand(time(0));
        return rand() % players.size();
    }

    void GameLogic::performTurn(Player* currentPlayer, Board& board, std::vector<Player*>& players) 
    {
        int diceRoll = rollDice() + rollDice();
        distributeResources(board, players, diceRoll);
        // Handle trading, building, and using development cards
    }

    int GameLogic::rollDice() 
    {
        return rand() % 6 + 1;
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