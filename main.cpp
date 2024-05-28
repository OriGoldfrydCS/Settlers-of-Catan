#include "catan.hpp"
#include "player.hpp"
#include "board.hpp"

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

    // Initialize game settings (placing initial settlements and roads)
    game.initializeGame();

    // Print state after initial placements
    game.printGameState();

    // Print players structures
    cout << player1.printPlayerStructures() << endl;
    cout << player3.printPlayerStructures() << endl;

    return 0;
}
