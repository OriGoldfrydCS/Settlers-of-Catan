#include "doctest.h"
#include "board.hpp"
#include "player.hpp"
#include "catan.hpp"
#include <sstream>

using namespace ariel;
using namespace std;

TEST_CASE("Tiles are correctly initialized") {
    Board board;
    CHECK_NOTHROW(board.getTile({0, 0}));
    CHECK(board.getTile({0, 0}).getResourceType() == ResourceType::NONE);
    CHECK(board.getTile({1, 0}).getResourceType() == ResourceType::WOOD);
}

TEST_CASE("Accessing an invalid tile throws exception") {
    Board board;
    CHECK_THROWS_AS(board.getTile({15, 15}), std::out_of_range);
    CHECK_THROWS_AS(board.getTile({-5, -5}), std::out_of_range);
    CHECK_THROWS_AS(board.getTile({-5, 5}), std::out_of_range);
    CHECK_THROWS_AS(board.getTile({5, -5}), std::out_of_range);
}

TEST_CASE("Check adjacency between valid intersections") {
    Board board;
    CHECK(board.areIntersectionsAdjacent(1, 9));
}

TEST_CASE("Check adjacency between non-adjacent intersections") {
    Board board;
    CHECK_FALSE(board.areIntersectionsAdjacent(1, 8));
}

TEST_CASE("Verify road connection to an intersection") {
    Board board;
    board.placeInitialRoad(Edge{Intersection::getIntersection(1), Intersection::getIntersection(9)}, 1);
    CHECK(board.isIntersectionConnectedToPlayerRoad(1, 1));
}

TEST_CASE("Verify no road connection where none exists") {
    Board board;
    CHECK_FALSE(board.isIntersectionConnectedToPlayerRoad(1, 2));
}

TEST_CASE("Initial settlement placement is successful") {
    Board board;
    CHECK_NOTHROW(board.placeInitialSettlement(1, 1));
    CHECK(board.hasSettlement(1));
}

TEST_CASE("Settlement placement follows rules") {
    Board board;
    board.placeInitialRoad(Edge{Intersection::getIntersection(1), Intersection::getIntersection(9)}, 1);
    CHECK(board.canPlaceSettlement(9, 1));
    board.placeSettlement(9, 1);
    CHECK(board.hasSettlement(9));
}

TEST_CASE("Initial road placement is successful") {
    Board board;
    CHECK_NOTHROW(board.placeInitialRoad(Edge{Intersection::getIntersection(1), Intersection::getIntersection(9)}, 1));
}

TEST_CASE("Road placement follows rules") {
    Board board;
    board.placeInitialSettlement(1, 1);
    CHECK(board.canPlaceRoad(Edge{Intersection::getIntersection(1), Intersection::getIntersection(9)}, 1));
    board.placeRoad(Edge{Intersection::getIntersection(1), Intersection::getIntersection(9)}, 1);
}

TEST_CASE("Resource distribution on initialization of the game") {
    Board board;
    Player player1("Avi"), player2("Beni"), player3("Choci");
    vector<Player*> players = {&player1, &player2, &player3};

    // Assuming Catan setup doesn't distribute resources initially
    Catan catan(player1, player2, player3); 
    catan.getBoard().setupTiles();  // Setup tiles manually for the test
    catan.getBoard().linkTilesAndIntersections(); // Link manually for the test

    // Place a settlement at an intersection known to be adjacent to an ORE tile
    players[0]->placeInitialSettlement(1, catan.getBoard());  
    players[1]->placeInitialSettlement(2, catan.getBoard());
    players[2]->placeInitialSettlement(4, catan.getBoard());

    // Ensure the settlement is placed correctly
    CHECK(catan.getBoard().hasSettlement(1));
    CHECK(catan.getBoard().hasSettlement(2));
    CHECK(catan.getBoard().hasSettlement(4));

    // Distribute resources manually for the test
    catan.distributeResources(&player1);
    catan.distributeResources(&player2);
    catan.distributeResources(&player3);

    // Check if the player received the expected ORE resource
    CHECK(player1.getResourceCount(ResourceType::ORE) == 1);
    CHECK(player2.getResourceCount(ResourceType::ORE) == 1);
    CHECK(player3.getResourceCount(ResourceType::WOOL) == 1);
}

TEST_CASE("Resource distribution based on dice roll (without distribution of initial reasorces)") {
    Board board;
    Player player1("Avi"), player2("Beno"), player3("Choci");
    vector<Player*> players = {&player1, &player2, &player3};

    Catan catan(player1, player2, player3);
    board.setupTiles();  
    board.linkTilesAndIntersections(); 

    players[0]->placeInitialSettlement(1, catan.getBoard());  
    players[1]->placeInitialSettlement(17, catan.getBoard());
    players[2]->placeInitialSettlement(36, catan.getBoard());

    int diceRoll = 8; 
    board.distributeResourcesBasedOnDiceRoll(diceRoll, players);

    // Check resource allocation based on the simulated dice roll
    CHECK(player1.getResourceCount(ResourceType::ORE) == 0); 
    CHECK(player1.getResourceCount(ResourceType::BRICK) == 0); 
    CHECK(player1.getResourceCount(ResourceType::WOOL) == 0); 
    CHECK(player1.getResourceCount(ResourceType::WOOD) == 0); 
    CHECK(player1.getResourceCount(ResourceType::GRAIN) == 0); 

    CHECK(player2.getResourceCount(ResourceType::ORE) == 0); 
    CHECK(player2.getResourceCount(ResourceType::BRICK) == 0); 
    CHECK(player2.getResourceCount(ResourceType::WOOL) == 0); 
    CHECK(player2.getResourceCount(ResourceType::WOOD) == 0); 
    CHECK(player2.getResourceCount(ResourceType::GRAIN) == 0); 

    CHECK(player3.getResourceCount(ResourceType::ORE) == 1); 
    CHECK(player3.getResourceCount(ResourceType::BRICK) == 0); 
    CHECK(player3.getResourceCount(ResourceType::WOOL) == 0); 
    CHECK(player3.getResourceCount(ResourceType::WOOD) == 0); 
    CHECK(player3.getResourceCount(ResourceType::GRAIN) == 0); 
}




TEST_CASE("Upgrading a settlement to city is successful") {
    Board board;
    int playerID = 1;
    board.placeInitialSettlement(1, playerID);
    CHECK(board.canUpgradeSettlementToCity(1, playerID));
    board.upgradeSettlementToCity(1, playerID);
    CHECK_FALSE(board.hasSettlement(1));
    CHECK(board.getCities().find(1) != board.getCities().end());
}

TEST_CASE("Add and retrieve resources") {
    Player player("Alice");
    player.addResource(ResourceType::WOOD, 5);
    CHECK(player.getResourceCount(ResourceType::WOOD) == 5);
}

TEST_CASE("Check resource sufficiency for building a road") {
    Player player("Alice");
    player.addResource(ResourceType::WOOD, 1);
    player.addResource(ResourceType::BRICK, 1);
    CHECK(player.canBuild("road"));
}

TEST_CASE("Build road and check resource decrement") {
    Player player("Alice");
    Board board;

    // Setup an initial road or settlement to ensure connectivity
    board.placeInitialSettlement(1, player.getId());
    player.placeSettlement(1, board);

    // Adding necessary resources
    player.addResource(ResourceType::WOOD, 1);
    player.addResource(ResourceType::BRICK, 1);

    // Attempt to build road
    player.buildRoad(Edge{Intersection::getIntersection(1), Intersection::getIntersection(2)}, board);

    // Check results
    CHECK(player.getResourceCount(ResourceType::WOOD) == 0);
    CHECK(player.getResourceCount(ResourceType::BRICK) == 0);
}

TEST_CASE("Alice cannot build an isolated road") {
    Player alice("Alice");
    Board board;

    // Alice has resources to build a road
    alice.addResource(ResourceType::WOOD, 1);
    alice.addResource(ResourceType::BRICK, 1);

    // Coordinates for isolated road that does not connect to any settlements or roads
    int isolatedStart = 15;
    int isolatedEnd = 16;

    // Before you build the road, you want to assert that there is no road present:
    CHECK_FALSE(board.isRoadPresent(isolatedStart, isolatedEnd));

    // Attempt to build road without any connecting settlements or roads
    alice.buildRoad(Edge{Intersection::getIntersection(isolatedStart), Intersection::getIntersection(isolatedEnd)}, board);

    // Check that resources are not deducted because the road should not be built
    CHECK(alice.getResourceCount(ResourceType::WOOD) == 1);
    CHECK(alice.getResourceCount(ResourceType::BRICK) == 1);

    // Check again with the expectation that there should still be no road if the rules were applied correctly:
    CHECK_FALSE(board.isRoadPresent(isolatedStart, isolatedEnd));
}


TEST_CASE("Build settlement and check resource decrement without road connection (by using modified function)") {
    Player player("Alice");
    Board board;

    // Add resources necessary for building a settlement
    player.addResource(ResourceType::WOOD, 1);
    player.addResource(ResourceType::BRICK, 1);
    player.addResource(ResourceType::WOOL, 1);
    player.addResource(ResourceType::GRAIN, 1);

    // Build settlement using the testing function that ignores road connection
    player.buildSettlementForTesting(1, board);

    // Check if resources have been decremented properly
    CHECK(player.getResourceCount(ResourceType::WOOD) == 0);
    CHECK(player.getResourceCount(ResourceType::BRICK) == 0);
    CHECK(player.getResourceCount(ResourceType::WOOL) == 0);
    CHECK(player.getResourceCount(ResourceType::GRAIN) == 0);
}

TEST_CASE("Build settlement fails without road connection in real game") {
    Player player("Alice");
    Board board;

    // Add resources necessary for building a settlement
    player.addResource(ResourceType::WOOD, 1);
    player.addResource(ResourceType::BRICK, 1);
    player.addResource(ResourceType::WOOL, 1);
    player.addResource(ResourceType::GRAIN, 1);

    // Attempt to build a settlement at an intersection without a connecting road
    player.buildSettlement(1, board);

    // Resources should not be decremented because the settlement should not be placed
    CHECK(player.getResourceCount(ResourceType::WOOD) == 1);
    CHECK(player.getResourceCount(ResourceType::BRICK) == 1);
    CHECK(player.getResourceCount(ResourceType::WOOL) == 1);
    CHECK(player.getResourceCount(ResourceType::GRAIN) == 1);

    // Ensure that no settlement was placed
    CHECK_FALSE(board.hasSettlement(1));
}



TEST_CASE("Upgrade settlement to city and verify point increase") {
    Player player("Alice");
    Board board;

    // Place initial settlement
    player.placeInitialSettlement(1, board);

    // Check if the settlement is placed correctly
    if (board.hasSettlement(1)) {
        std::cout << "Settlement correctly placed at intersection 1." << std::endl;
    } else {
        std::cout << "Failed to place settlement at intersection 1." << std::endl;
    }

    // Add resources necessary for upgrading to a city
    player.addResource(ResourceType::ORE, 3);
    player.addResource(ResourceType::GRAIN, 2);

    // Check resource counts before upgrading
    std::cout << "Resources before upgrading:" << std::endl;
    std::cout << "ORE: " << player.getResourceCount(ResourceType::ORE) << std::endl;
    std::cout << "GRAIN: " << player.getResourceCount(ResourceType::GRAIN) << std::endl;

    // Upgrade the settlement to a city
    player.upgradeToCity(1, board);

    // Check if the player points are updated correctly
    std::cout << "Player points after upgrading: " << player.getPoints() << std::endl;
    CHECK(player.getPoints() == 2);  // Assuming 1 point for initial settlement, additional 1 for city

    // Verify if the city is registered
    if (board.getCities().find(1) != board.getCities().end()) {
        std::cout << "City correctly registered at intersection 1." << std::endl;
        CHECK(board.getCities().find(1) != board.getCities().end());
    } else {
        std::cout << "No city registered at intersection 1." << std::endl;
    }
}


TEST_CASE("Player initialization assigns unique ID") {
    Player player1("Alice");
    Player player2("Bob");
    CHECK(player1.getId() != player2.getId());
}

// TEST_CASE("Trade resources between players") {
//     Player alice("Alice");
//     Player bob("Bob");
//     Board board;
//     vector<Player*> players = {&alice, &bob};
//     alice.addResource(ResourceType::WOOD, 5);
//     bob.addResource(ResourceType::BRICK, 5);
//     alice.trade(players);
//     bob.trade(players);
//     CHECK(alice.getResourceCount(ResourceType::BRICK) > 0);
//     CHECK(bob.getResourceCount(ResourceType::WOOD) > 0);
// }


// TESTS FOR PLAYER
TEST_CASE("Buy and use a development card") {
    Player alice("Alice");
    alice.addResource(ResourceType::ORE, 1);
    alice.addResource(ResourceType::WOOL, 1);
    alice.addResource(ResourceType::GRAIN, 1);
    std::vector<Player*> allPlayers{&alice};
    CardPurchaseError result = alice.buyDevelopmentCard(DevCardType::KNIGHT, allPlayers);
    CHECK(result == CardPurchaseError::Success);
    const auto& developmentCards = alice.getDevelopmentCards();
    auto it = developmentCards.find(DevCardType::KNIGHT);
    CHECK(it != developmentCards.end());
    CHECK(it->second == 1);
}

TEST_CASE("Using a Victory Point card increases points and decreases the count") {
    Player alice("Alice");

    // Adding sufficient resources to purchase a victory point card
    alice.addResource(ResourceType::ORE, 1);
    alice.addResource(ResourceType::WOOL, 1);
    alice.addResource(ResourceType::GRAIN, 1);

    std::vector<Player*> allPlayers{&alice};

    // Buy a victory point card
    CHECK(alice.buyDevelopmentCard(DevCardType::VICTORY_POINT, allPlayers) == CardPurchaseError::Success);

    Board board;

    // Initially set endTurn to false to check if using the card sets it to true
    bool endTurn = false;

    // Use the victory point card
    auto result = alice.useDevelopmentCard(DevCardType::VICTORY_POINT, allPlayers, board, endTurn);

    // Checks
    CHECK(result == CardUseError::Success);
    CHECK(alice.getDevelopmentCards().at(DevCardType::VICTORY_POINT) == 0); // Check that the card count has decreased
    CHECK(endTurn == true);  // Check if using a victory point card should end the turn
    CHECK(alice.getPoints() == 1);  // Check if points increased by 1
}


TEST_CASE("Trading resources between players") {
    Player alice("Alice"), bob("Bob");
    vector<Player*> players = {&alice, &bob};
    alice.addResource(ResourceType::WOOD, 5);
    bob.addResource(ResourceType::ORE, 5);

    // Simulate input for selecting Bob, trading 3 wood for 3 ore, and Bob accepting the trade
    std::stringstream input("1\n2\n3 0 0 0 0\n0 0 0 0 3\nyes\n");
    std::cin.rdbuf(input.rdbuf()); // Redirect std::cin to read from input

    // Call the trade function
    alice.trade(players);

    // Check the results of the trade
    CHECK(alice.getResourceCount(ResourceType::WOOD) == 2); // Alice should have 2 wood left
    CHECK(alice.getResourceCount(ResourceType::ORE) == 3);  // Alice should have 3 ore
    CHECK(bob.getResourceCount(ResourceType::WOOD) == 3);   // Bob should have 3 wood
    CHECK(bob.getResourceCount(ResourceType::ORE) == 2);    // Bob should have 2 ore

    // Restore cin to standard input before finishing
    std::cin.rdbuf(nullptr);
}


TEST_CASE("Player trades Knight card and loses Largest Army status") {

    Player alice("Alice"), bob("Bob");

    vector<Player*> players = {&alice, &bob};

    // Alice buys 3 Knight cards
    alice.addResource(ResourceType::ORE, 3);
    alice.addResource(ResourceType::WOOL, 3);
    alice.addResource(ResourceType::GRAIN, 3);

    for (int i = 0; i < 3; i++) {
        CHECK(alice.buyDevelopmentCard(DevCardType::KNIGHT, players) == CardPurchaseError::Success);
    }

    Player::largestArmyHolder = &alice;  // Assume Alice initially has the largest army
    alice.addPoints(2);  // Assume she has 2 points for holding the largest army

    // Ensure Alice now has the Largest Army
    CHECK(alice.getDevelopmentCards().at(DevCardType::KNIGHT) == 3);
    CHECK(Player::largestArmyHolder == &alice);
    CHECK(alice.getPoints() == 4); // Points for largest army (2+2)

    std::cout << "Before trade:" << std::endl;
    std::cout << "Alice Knight cards: " << alice.getDevelopmentCards().at(DevCardType::KNIGHT) << std::endl;
    std::cout << "Bob Knight cards: " << bob.getDevelopmentCards().at(DevCardType::KNIGHT) << std::endl;
    std::cout << "Alice points: " << alice.getPoints() << std::endl;
    std::cout << "Largest Army Holder: " << (Player::largestArmyHolder == &alice ? "Alice" : "Not Alice") << std::endl;

    // Simulate a card trade where Alice trades away one Knight card
    std::stringstream input("2\n1\n1 0 0 0 0\n0 0 0 0 0\nyes\n"); // Simulate input for trading one Knight card
    std::cin.rdbuf(input.rdbuf()); // Redirect std::cin to read from input

    // Assuming Bob has the necessary resources to accept the trade
    bob.addResource(ResourceType::ORE, 1);
    bob.addResource(ResourceType::WOOL, 1);
    bob.addResource(ResourceType::GRAIN, 1);
    bob.buyDevelopmentCard(DevCardType::KNIGHT, players);

    alice.tradeCards(players);

    std::cin.rdbuf(nullptr); // Restore cin to standard input

    // Check if Alice lost a Knight card
    std::cout << "After trade:" << std::endl;
    std::cout << "Alice Knight cards: " << alice.getDevelopmentCards().at(DevCardType::KNIGHT) << std::endl;
    std::cout << "Bob Knight cards: " << bob.getDevelopmentCards().at(DevCardType::KNIGHT) << std::endl;
    std::cout << "Alice points: " << alice.getPoints() << std::endl;
    std::cout << "Largest Army Holder: " << (Player::largestArmyHolder == &alice ? "Alice" : "Not Alice") << std::endl;

    CHECK(alice.getDevelopmentCards().at(DevCardType::KNIGHT) == 2);
    CHECK(bob.getDevelopmentCards().at(DevCardType::KNIGHT) == 1); // Bob should have 1 Knight card

    // Check if Alice still holds the Largest Army (this depends on game logic - she may need to lose the status actively)
    alice.checkForLargestArmy(players);

    // Verify that Alice no longer holds the largest army if another player meets the criteria
    std::cout << "After checking for largest army:" << std::endl;
    std::cout << "Alice points: " << alice.getPoints() << std::endl;
    std::cout << "Largest Army Holder: " << (Player::largestArmyHolder == &alice ? "Alice" : "Not Alice") << std::endl;

    CHECK(Player::largestArmyHolder != &alice);
    CHECK(alice.getPoints() == 2); // Points lost due to losing largest army
}





TEST_CASE("Resource usage for building structures") {
    Player player("Alice");
    Board board;

    // Add resources and build a road
    player.addResource(ResourceType::WOOD, 2);
    player.addResource(ResourceType::BRICK, 2);

    player.placeInitialSettlement(1, board);
    player.buildRoad(Edge{Intersection::getIntersection(1), Intersection::getIntersection(2)}, board);

    CHECK(player.getResourceCount(ResourceType::WOOD) == 1);
    CHECK(player.getResourceCount(ResourceType::BRICK) == 1);

    player.buildRoad(Edge{Intersection::getIntersection(2), Intersection::getIntersection(3)}, board);

    // Add resources and build a settlement
    player.addResource(ResourceType::WOOD, 1);
    player.addResource(ResourceType::BRICK, 1);
    player.addResource(ResourceType::WOOL, 1);
    player.addResource(ResourceType::GRAIN, 1);

    player.buildSettlement(3, board);

    CHECK(player.getResourceCount(ResourceType::WOOD) == 0);
    CHECK(player.getResourceCount(ResourceType::BRICK) == 0);
    CHECK(player.getResourceCount(ResourceType::WOOL) == 0);
    CHECK(player.getResourceCount(ResourceType::GRAIN) == 0);
}

TEST_CASE("Points calculation for building settlements and cities") {
    Player player("Alice");
    Board board;

    // Place initial settlement
    player.placeInitialSettlement(1, board);
    CHECK(player.getPoints() == 1);

    // Add resources for upgrading to a city
    player.addResource(ResourceType::ORE, 3);
    player.addResource(ResourceType::GRAIN, 2);

    // Upgrade to a city
    player.upgradeToCity(1, board);
    CHECK(player.getPoints() == 2);
}
