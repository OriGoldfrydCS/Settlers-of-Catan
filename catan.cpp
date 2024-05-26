#include "catan.hpp"
#include "vertex_edge.hpp"
#include <iostream>
#include <stdexcept>

using namespace std;
namespace ariel {


    // Constructor
    Catan::Catan(Player& p1, Player& p2, Player& p3) : players({&p1, &p2, &p3}), currentPlayerIndex(0) 
    {
        initializeGame();
    }

    void Catan::initializeGame() 
    {
        board.setupTiles();  
        board.linkTilesAndIntersections();

        // Blue (1st) Player Setup:
        players[0]->placeSettlement(29, board);     // Intersection 29 touches Wood(8), Ore(3), Brick(5)
        players[0]->placeSettlement(33, board);     // Intersection 33 touches Grain(4), Wool(5), Wool(11)
        players[0]->placeRoad(Edge(Intersection::getIntersection(29), Intersection::getIntersection(30)), board);   // Road between Ore(3) and Brick(5)
        players[0]->placeRoad(Edge(Intersection::getIntersection(25), Intersection::getIntersection(33)), board);  // Road between Grain(4) and Wool(5)

        // Yellow (2nd) Player Setup:
        players[1]->placeSettlement(8, board);      // Intersection 8 touches Brick(10), Wood(9), Wool(4)
        players[1]->placeSettlement(31, board);     // Intersection 31 touches Ore(3), Grain(4), Grain(6)
        players[1]->placeRoad(Edge(Intersection::getIntersection(7), Intersection::getIntersection(8)), board);   // Road between Wool(4) and Wood(9)
        players[1]->placeRoad(Edge(Intersection::getIntersection(31), Intersection::getIntersection(32)), board); // Road between Grain(4) and Grain(6)

        // White (3rd) Player Setup:
        players[2]->placeSettlement(12, board);     // Intersection 12 touches Grain(12), Brick(6), Wood(11)
        players[2]->placeSettlement(26, board);     // Intersection 26 touches Wood(3), Ore(8), Wool(5)
        players[2]->placeRoad(Edge(Intersection::getIntersection(11), Intersection::getIntersection(12)), board);     // Road between Grain(12) and Wood(11)
        players[2]->placeRoad(Edge(Intersection::getIntersection(17), Intersection::getIntersection(26)), board);     // Road between Wood(3) and Ore(8)

        ChooseStartingPlayer();  
    }

    void Catan::ChooseStartingPlayer() 
    {
        // Implement
    }
    
    // void Catan::playGame() 
    // {
    //     while (true) 
    //     {
    //         Player* currentPlayer = players[currentPlayerIndex];
    //         GameLogic::performTurn(currentPlayer, board);
    //         if (GameLogic::checkVictoryConditions(currentPlayer)) 
    //         {
    //             cout << "Player " << currentPlayer->getPoints() << " wins!" << endl;
    //             break;
    //         }
    //         nextTurn();
    //     }
    //     // Implement the main game loop, controlling the flow of turns and checking for the game end condition
    // }

    // void Catan::nextTurn() 
    // {
    //     currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
    //     // Implement the logic to advance the game to the next player's turn
    // }

    void Catan::printGameState() const 
    {
        cout << "Game state:" << endl;
        for (const auto& player : players) 
        {
            player->printPoints();
        }
        board.printBoard();
        // Implement the logic to display the current state of the game
    }

    Board& Catan::getBoard() 
    {
        return this->board;
    }

    void Catan::printWinner() 
    {
        Player* winner = nullptr;
        int maxPoints = 0;
        for (const auto& player : players) 
        {
            if (player->getPoints() > maxPoints) 
            {
                maxPoints = player->getPoints();
                winner = player;
            }
        }
        if (winner) 
        {
            cout << "Winner: " << winner->getPoints() << " points" << endl;
        }
        // Implement the logic to print the winner of the game
    }
}