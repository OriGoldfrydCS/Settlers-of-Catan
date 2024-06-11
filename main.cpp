// Email: origoldbsc@gmail.com

#include "catan.hpp"
#include <iostream>
#include <string>
#include <sstream>

using namespace std;
using namespace ariel;

int main() {

    // Create players
    Player player1("Player 1");
    Player player2("Player 2");
    Player player3("Player 3");

    // Create the game
    Catan game(player1, player2, player3);

    // Initialize the game
    game.initializeGame();

    cout << "Starting the game...\n";

    // Player 1's turn //
    cout << "It is now Player 1's turn.\n";

    cout << player1.printPlayer() << endl;

    // Provide input for building a road
    string RoadInput = "42 50";
    stringstream RoadInputStream(RoadInput);
    cin.rdbuf(RoadInputStream.rdbuf());
    
    game.handleBuildRoad(&player1);

    cout << player1.printPlayer() << endl;

    // game.handleBuildSettlement(&player1);   // To build road we need 1 BRICK, 1 WOOD, 1 WOOL and 1 GRAIN (impossible in out case)
    player1.endTurn();
    game.nextTurn();

    // Player 2's turn //
    
    // Provide input for buying a development card
    string DevCardInput = "2";           // Buying a Victory Point card
    stringstream DevCardStream(DevCardInput);
    cin.rdbuf(DevCardStream.rdbuf());
    game.handleBuyDevelopmentCard(&player2);
    
    cout << player2.printPlayer() << endl;

    string DevCardUsage = "1";           // Using a Victory Point card
    stringstream DevCardUsageStream(DevCardUsage);
    cin.rdbuf(DevCardUsageStream.rdbuf());
    bool shouldEndTurn = false;
    game.handleDevelopmentCardUsage(&player2, shouldEndTurn);

    cout << player2.printPlayer() << endl;

    player2.endTurn();
    game.nextTurn();

    // Player 3's turn //

    // Provide input for trading
    std::stringstream input("1\n2\n0 0 1 0 0\n1 1 0 0 0\nyes\n");       // Trade WOOL for WOOD and BRICK
    std::cin.rdbuf(input.rdbuf()); 
    player3.trade(game.getPlayers());   
    
    cout << player3.printPlayer() << endl;

    player3.endTurn();
    game.nextTurn();

    // Print the game state after one round
    game.printGameState();

    // Check if someone has won (unlikely after one round)
    game.hasWinner();

    return 0;
    
}