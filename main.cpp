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

    // Print state after initial placements
    game.printGameState();

    // Print players structures
    cout << player1.printPlayer() << endl;
    cout << player2.printPlayer() << endl;
    cout << player3.printPlayer() << endl;

    game.ChooseStartingPlayer();  

    game.playGame();

    return 0;
}
