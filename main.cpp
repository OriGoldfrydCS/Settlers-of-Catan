#include "catan.hpp"
#include "player.hpp"
#include <iostream>

using namespace ariel;
using namespace std;

int main() {
    // Create player instances
    Player player1("Blue");
    Player player2("Yellow");
    Player player3("White");

    // Initialize the game with these players
    Catan game(player1, player2, player3);

    // Print the initial state of the board
    cout << "Initial Board State:\n" << endl;
    game.getBoard().printBoard();

    // Initialize game settings (placing initial settlements and roads)
    game.initializeGame();

    // Print state after initial placements
    cout << "Board State After Initial Placements:\n";
    game.printGameState();

    return 0;
}
