// Email: origoldbsc@gmail.com

#include "doctest.h"
#include "board.hpp"
#include "player.hpp"
#include "catan.hpp"
#include <sstream>

using namespace ariel;
using namespace std;


/*********************************************/
///             TESTS FOR BOARD             ///
/*********************************************/

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

TEST_CASE("Test areIntersectionsAdjacent function") {
    Board board;
    CHECK(board.areIntersectionsAdjacent(1, 2));
    CHECK(board.areIntersectionsAdjacent(2, 3));
    CHECK_FALSE(board.areIntersectionsAdjacent(1, 5));
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

TEST_CASE("Test canPlaceSettlement function with invalid cases") {
    Board board;
    board.placeInitialSettlement(22, 1);
    board.placeInitialRoad(Edge{Intersection::getIntersection(21), Intersection::getIntersection(22)}, 1);
    CHECK_FALSE(board.canPlaceSettlement(2, 0));
    CHECK_FALSE(board.canPlaceSettlement(100, 0));
}

TEST_CASE("Test getTile function") {
    Board board;
    CHECK_NOTHROW(board.getTile({0, 2}));
    CHECK_THROWS_AS(board.getTile({10, 10}), std::out_of_range);
}

TEST_CASE("Test getIntersectionID function + invalid intersection") {
    Board board;
    Intersection i1 = Intersection::getIntersection(1);
    CHECK(board.getIntersectionID(i1) == 1);
    Intersection i2 = Intersection::getIntersection(54);
    CHECK(board.getIntersectionID(i2) == 54);
    REQUIRE_THROWS_WITH(Intersection::getIntersection(100), "Invalid intersection ID: 100");
}

TEST_CASE("Test getResourceTypesAroundIntersection function") {
    Board board;
    CHECK(board.getResourceTypesAroundIntersection(1) == std::vector<ResourceType>({ResourceType::ORE}));
    CHECK(board.getResourceTypesAroundIntersection(54) == std::vector<ResourceType>({ResourceType::WOOL}));
}

TEST_CASE("Test canUpgradeSettlementToCity function") {
    Board board;
    board.placeInitialSettlement(1, 0);
    CHECK(board.canUpgradeSettlementToCity(1, 0));
    CHECK(!board.canUpgradeSettlementToCity(2, 0));
}


/*********************************************/
///             TESTS FOR BOARD             ///
/*********************************************/

TEST_CASE("Tests for devCardTypeToString function") {
    CHECK(ariel::devCardTypeToString(ariel::DevCardType::PROMOTION) == "Promotion");
    CHECK(ariel::devCardTypeToString(ariel::DevCardType::KNIGHT) == "Knight");
    CHECK(ariel::devCardTypeToString(ariel::DevCardType::VICTORY_POINT) == "Victory Point");
    CHECK(ariel::devCardTypeToString(static_cast<ariel::DevCardType>(100)) == "Unknown");
}

TEST_CASE("Tests for promotionTypeToString function") {
    CHECK(ariel::promotionTypeToString(ariel::PromotionType::MONOPOLY) == "Monopoly");
    CHECK(ariel::promotionTypeToString(ariel::PromotionType::ROAD_BUILDING) == "Road Building");
    CHECK(ariel::promotionTypeToString(ariel::PromotionType::YEAR_OF_PLENTY) == "Year of Plenty");
    CHECK(ariel::promotionTypeToString(static_cast<ariel::PromotionType>(100)) == "Unknown");
}

/*********************************************/
///             TESTS FOR PLAYER            ///
/*********************************************/

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
    Player player("Avi");
    player.addResource(ResourceType::WOOD, 5);
    CHECK(player.getResourceCount(ResourceType::WOOD) == 5);
}

TEST_CASE("Check resource sufficiency for building a road") {
    Player player("Avi");
    player.addResource(ResourceType::WOOD, 1);
    player.addResource(ResourceType::BRICK, 1);
    CHECK(player.canBuild("road"));
}

TEST_CASE("Build road and check resource decrement") {
    Player player("Avi");
    Board board;

    // Setup an initial road or settlement to ensure connectivity
    board.placeInitialSettlement(1, player.getId());
    player.buildSettlement(1, board);

    // Adding necessary resources
    player.addResource(ResourceType::WOOD, 1);
    player.addResource(ResourceType::BRICK, 1);

    // Attempt to build road
    player.buildRoad(Edge{Intersection::getIntersection(1), Intersection::getIntersection(2)}, board);

    // Check results
    CHECK(player.getResourceCount(ResourceType::WOOD) == 0);
    CHECK(player.getResourceCount(ResourceType::BRICK) == 0);
}

TEST_CASE("Ami cannot build an isolated road") {
    Player ami("Ami");
    Board board;

    // Avi has resources to build a road
    ami.addResource(ResourceType::WOOD, 1);
    ami.addResource(ResourceType::BRICK, 1);

    // Coordinates for isolated road that does not connect to any settlements or roads
    int isolatedStart = 15;
    int isolatedEnd = 16;

    // Before you build the road, you want to assert that there is no road present:
    CHECK_FALSE(board.isRoadPresent(isolatedStart, isolatedEnd));

    // Attempt to build road without any connecting settlements or roads
    ami.buildRoad(Edge{Intersection::getIntersection(isolatedStart), Intersection::getIntersection(isolatedEnd)}, board);

    // Check that resources are not deducted because the road should not be built
    CHECK(ami.getResourceCount(ResourceType::WOOD) == 1);
    CHECK(ami.getResourceCount(ResourceType::BRICK) == 1);

    // Check again with the expectation that there should still be no road if the rules were applied correctly:
    CHECK_FALSE(board.isRoadPresent(isolatedStart, isolatedEnd));
}


TEST_CASE("Build settlement and check resource decrement without road connection (by using modified function)") {
    Player player("Ami");
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
    Player player("Ani");
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
    Player player("Ami");
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
    Player player1("Eli");
    Player player2("Dean");
    CHECK(player1.getId() != player2.getId());
}


// TESTS FOR PLAYER
TEST_CASE("Buy and use a development card") {
    Player ami("Ami");
    ami.addResource(ResourceType::ORE, 1);
    ami.addResource(ResourceType::WOOL, 1);
    ami.addResource(ResourceType::GRAIN, 1);
    std::vector<Player*> allPlayers{&ami};
    CardPurchaseError result = ami.buyDevelopmentCard(DevCardType::KNIGHT, allPlayers);
    CHECK(result == CardPurchaseError::Success);
    const auto& developmentCards = ami.getDevelopmentCards();
    auto it = developmentCards.find(DevCardType::KNIGHT);
    CHECK(it != developmentCards.end());
    CHECK(it->second == 1);
}

TEST_CASE("Using a Victory Point card increases points and decreases the count") {
    Player adi("Adi");

    // Adding sufficient resources to purchase a victory point card
    adi.addResource(ResourceType::ORE, 1);
    adi.addResource(ResourceType::WOOL, 1);
    adi.addResource(ResourceType::GRAIN, 1);

    std::vector<Player*> allPlayers{&adi};

    // Buy a victory point card
    CHECK(adi.buyDevelopmentCard(DevCardType::VICTORY_POINT, allPlayers) == CardPurchaseError::Success);

    Board board;

    // Initially set endTurn to false to check if using the card sets it to true
    bool endTurn = false;

    // Use the victory point card
    auto result = adi.useDevelopmentCard(DevCardType::VICTORY_POINT, allPlayers, board, endTurn);

    // Checks
    CHECK(result == CardUseError::Success);
    CHECK(adi.getDevelopmentCards().at(DevCardType::VICTORY_POINT) == 0); // Check that the card count has decreased
    CHECK(endTurn == true);  // Check if using a victory point card should end the turn
    CHECK(adi.getPoints() == 1);  // Check if points increased by 1
}


TEST_CASE("Trading resources between players") {
    Player adi("Adi"), bil("Bil");
    vector<Player*> players = {&adi, &bil};
    adi.addResource(ResourceType::WOOD, 5);
    bil.addResource(ResourceType::ORE, 5);

    // Simulate input for selecting Bil, trading 3 wood for 3 ore, and Bil accepting the trade
    std::stringstream input("1\n2\n3 0 0 0 0\n0 0 0 0 3\nyes\n");
    std::cin.rdbuf(input.rdbuf()); // Redirect std::cin to read from input

    // Call the trade function
    adi.trade(players);

    // Check the results of the trade
    CHECK(adi.getResourceCount(ResourceType::WOOD) == 2); // Adi should have 2 wood left
    CHECK(adi.getResourceCount(ResourceType::ORE) == 3);  // Adi should have 3 ore
    CHECK(bil.getResourceCount(ResourceType::WOOD) == 3);   // Bil should have 3 wood
    CHECK(bil.getResourceCount(ResourceType::ORE) == 2);    // Bil should have 2 ore

    // Restore cin to standard input before finishing
    std::cin.rdbuf(nullptr);
}


TEST_CASE("Player loses Largest Army card to someone else") {
    Player adi("Adi"), bil("Bil");
    vector<Player*> players = {&adi, &bil};

    // Adi buys 3 Knight cards
    adi.addResource(ResourceType::ORE, 3);
    adi.addResource(ResourceType::WOOL, 3);
    adi.addResource(ResourceType::GRAIN, 3);

    cout << "Initial Resources:" << endl;
    adi.printResources();
    bil.printResources();

    for (int i = 0; i < 3; i++) {
        CHECK(adi.buyDevelopmentCard(DevCardType::KNIGHT, players) == CardPurchaseError::Success);
    }

    Player::largestArmyHolder = &adi;  // Assume Adi initially has the largest army
    adi.addPoints(2);  // Assume she has 2 points for holding the largest army

    // Ensure Adi now has the Largest Army
    CHECK(adi.getDevelopmentCards().at(DevCardType::KNIGHT) == 3);
    CHECK(Player::largestArmyHolder == &adi);
    CHECK(adi.getPoints() == 4); // Points for largest army (2+2)


    bil.addResource(ResourceType::ORE, 6);
    bil.addResource(ResourceType::WOOL, 6);
    bil.addResource(ResourceType::GRAIN, 6);

    // Bil buys 4 Knight cards
    for (int i = 0; i < 4; i++) {
        CHECK(bil.buyDevelopmentCard(DevCardType::KNIGHT, players) == CardPurchaseError::Success);
    }

    adi.checkForLargestArmy(players);

    // Verify that Adi no longer holds the largest army if another player meets the criteria
    CHECK(Player::largestArmyHolder != &adi);
    CHECK(adi.getPoints() == 2); // Points lost due to losing largest army
}


TEST_CASE("Resource usage for building structures") {
    Player player("Ami");
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
    Player player("Ami");
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


/*********************************************/
///             TESTS FOR CATAN             ///
/*********************************************/



