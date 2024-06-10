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
    Board& board = Board::getInstance();
    CHECK_NOTHROW(board.getTile({0, 0}));
    CHECK(board.getTile({0, 0}).getResourceType() == ResourceType::NONE);
    CHECK(board.getTile({1, 0}).getResourceType() == ResourceType::WOOD);
}

TEST_CASE("Accessing an invalid tile throws exception") {
    Board& board = Board::getInstance();
    CHECK_THROWS_AS(board.getTile({15, 15}), out_of_range);
    CHECK_THROWS_AS(board.getTile({-5, -5}), out_of_range);
    CHECK_THROWS_AS(board.getTile({-5, 5}), out_of_range);
    CHECK_THROWS_AS(board.getTile({5, -5}), out_of_range);
}

TEST_CASE("Test areIntersectionsAdjacent function") {
    Board& board = Board::getInstance();
    CHECK(board.areIntersectionsAdjacent(1, 2));
    CHECK(board.areIntersectionsAdjacent(2, 3));
    CHECK_FALSE(board.areIntersectionsAdjacent(1, 5));
}

TEST_CASE("Verify road connection to an intersection") {
    Board& board = Board::getInstance();
    board.placeInitialRoad(Edge{Intersection::getIntersection(1), Intersection::getIntersection(9)}, 1);
    CHECK(board.isIntersectionConnectedToPlayerRoad(1, 1));
}

TEST_CASE("Verify no road connection where none exists") {
    Board& board = Board::getInstance();
    CHECK_FALSE(board.isIntersectionConnectedToPlayerRoad(1, 2));
}

TEST_CASE("Initial settlement placement is successful") {
    Board& board = Board::getInstance();
    CHECK_NOTHROW(board.placeInitialSettlement(1, 1));
    CHECK(board.hasSettlement(1));
}

TEST_CASE("Settlement placement follows rules") {
    Board& board = Board::getInstance();
    board.placeInitialRoad(Edge{Intersection::getIntersection(1), Intersection::getIntersection(9)}, 1);
    CHECK(board.canPlaceSettlement(9, 1));
    board.placeSettlement(9, 1);
    CHECK(board.hasSettlement(9));
}

TEST_CASE("Initial road placement is successful") {
    Board& board = Board::getInstance();
    CHECK_NOTHROW(board.placeInitialRoad(Edge{Intersection::getIntersection(1), Intersection::getIntersection(9)}, 1));
}

TEST_CASE("Road placement follows rules") {
    Board& board = Board::getInstance();
    board.placeInitialSettlement(1, 1);
    CHECK(board.canPlaceRoad(Edge{Intersection::getIntersection(1), Intersection::getIntersection(9)}, 1));
    board.placeRoad(Edge{Intersection::getIntersection(1), Intersection::getIntersection(9)}, 1);
}

TEST_CASE("Test canPlaceSettlement function with invalid cases") {
    Board& board = Board::getInstance();
    board.placeInitialSettlement(22, 1);
    board.placeInitialRoad(Edge{Intersection::getIntersection(21), Intersection::getIntersection(22)}, 1);
    CHECK_FALSE(board.canPlaceSettlement(2, 0));
    CHECK_FALSE(board.canPlaceSettlement(100, 0));
}

TEST_CASE("Test getTile function") {
    Board& board = Board::getInstance();
    CHECK_NOTHROW(board.getTile({0, 2}));
    CHECK_THROWS_AS(board.getTile({10, 10}), out_of_range);
}

TEST_CASE("Test getIntersectionID function + invalid intersection") {
    Board& board = Board::getInstance();
    Intersection i1 = Intersection::getIntersection(1);
    CHECK(board.getIntersectionID(i1) == 1);
    Intersection i2 = Intersection::getIntersection(54);
    CHECK(board.getIntersectionID(i2) == 54);
    REQUIRE_THROWS_WITH(Intersection::getIntersection(100), "Invalid intersection ID: 100");
}

TEST_CASE("Test getResourceTypesAroundIntersection function") {
    Board& board = Board::getInstance();
    CHECK(board.getResourceTypesAroundIntersection(1) == vector<ResourceType>({ResourceType::ORE}));
    CHECK(board.getResourceTypesAroundIntersection(54) == vector<ResourceType>({ResourceType::WOOL}));
}

TEST_CASE("Test canUpgradeSettlementToCity function") {
    Board& board = Board::getInstance();
    board.placeInitialSettlement(1, 0);
    CHECK(board.canUpgradeSettlementToCity(1, 0));
    CHECK(!board.canUpgradeSettlementToCity(2, 0));
}


/*********************************************/
///             TESTS FOR BOARD             ///
/*********************************************/

TEST_CASE("Tests for devCardTypeToString function") {
    CHECK(devCardTypeToString(DevCardType::PROMOTION) == "Promotion");
    CHECK(devCardTypeToString(DevCardType::KNIGHT) == "Knight");
    CHECK(devCardTypeToString(DevCardType::VICTORY_POINT) == "Victory Point");
    CHECK(devCardTypeToString(static_cast<DevCardType>(100)) == "Unknown");
}

TEST_CASE("Tests for promotionTypeToString function") {
    CHECK(promotionTypeToString(PromotionType::MONOPOLY) == "Monopoly");
    CHECK(promotionTypeToString(PromotionType::ROAD_BUILDING) == "Road Building");
    CHECK(promotionTypeToString(PromotionType::YEAR_OF_PLENTY) == "Year of Plenty");
    CHECK(promotionTypeToString(static_cast<PromotionType>(100)) == "Unknown");
}

/*********************************************/
///             TESTS FOR PLAYER            ///
/*********************************************/

TEST_CASE("Resource distribution on initialization of the game") {
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
    Board& board = Board::getInstance();
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
    Board& board = Board::getInstance();
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
    Board& board = Board::getInstance();

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
    Board& board = Board::getInstance();

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
    Board& board = Board::getInstance();

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
    Board& board = Board::getInstance();

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
    Board& board = Board::getInstance();

    // Place initial settlement
    player.placeInitialSettlement(1, board);

    // Check if the settlement is placed correctly
    if (board.hasSettlement(1)) {
        cout << "Settlement correctly placed at intersection 1." << endl;
    } else {
        cout << "Failed to place settlement at intersection 1." << endl;
    }

    // Add resources necessary for upgrading to a city
    player.addResource(ResourceType::ORE, 3);
    player.addResource(ResourceType::GRAIN, 2);

    // Check resource counts before upgrading
    cout << "Resources before upgrading:" << endl;
    cout << "ORE: " << player.getResourceCount(ResourceType::ORE) << endl;
    cout << "GRAIN: " << player.getResourceCount(ResourceType::GRAIN) << endl;

    // Upgrade the settlement to a city
    player.upgradeToCity(1, board);

    // Check if the player points are updated correctly
    cout << "Player points after upgrading: " << player.getPoints() << endl;
    CHECK(player.getPoints() == 2);  // Assuming 1 point for initial settlement, additional 1 for city

    // Verify if the city is registered
    if (board.getCities().find(1) != board.getCities().end()) 
    {
        cout << "City correctly registered at intersection 1." << endl;
        CHECK(board.getCities().find(1) != board.getCities().end());
    } 
    else 
    {
        cout << "No city registered at intersection 1." << endl;
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
    vector<Player*> allPlayers{&ami};
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

    vector<Player*> allPlayers{&adi};

    // Buy a victory point card
    CHECK(adi.buyDevelopmentCard(DevCardType::VICTORY_POINT, allPlayers) == CardPurchaseError::Success);

    Board& board = Board::getInstance();

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
    stringstream input("1\n2\n3 0 0 0 0\n0 0 0 0 3\nyes\n");
    cin.rdbuf(input.rdbuf()); 

    // Call the trade function
    adi.trade(players);

    // Check the results of the trade
    CHECK(adi.getResourceCount(ResourceType::WOOD) == 2); // Adi should have 2 wood left
    CHECK(adi.getResourceCount(ResourceType::ORE) == 3);  // Adi should have 3 ore
    CHECK(bil.getResourceCount(ResourceType::WOOD) == 3);   // Bil should have 3 wood
    CHECK(bil.getResourceCount(ResourceType::ORE) == 2);    // Bil should have 2 ore

    cin.rdbuf(nullptr);
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
    Board& board = Board::getInstance();

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
    Board& board = Board::getInstance();

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

TEST_CASE("initializeGame sets up players and board correctly") {
    Player p1("Ami"), p2("Avi"), p3("Ali");
    Catan game(p1, p2, p3);
    game.initializeGame();
    
    // Check if players have correct resources after initialization
    CHECK(p1.getResourceCount(ResourceType::WOOD) > 0);
    CHECK(p2.getResourceCount(ResourceType::BRICK) > 0);
    CHECK(p3.getResourceCount(ResourceType::WOOL) > 0);
    
    // Check for correct placement of settlements and roads
    CHECK(game.getBoard().hasSettlement(41));
    CHECK(game.getBoard().isRoadPresent(41, 42));
}

TEST_CASE("distributeResources gives correct resources") {
    Player p1("Ami"), p2("Avi"), p3("Ali");
    Catan game(p1, p2, p3);         
    game.testInitialize();          // Sets up board without placing settlements
    
    // Manually place a settlement for testing
    p1.placeInitialSettlement(1, game.getBoard());
    game.distributeResources(&p1);
    
    auto expectedResources = game.getBoard().getResourceTypesAroundIntersection(1);
    for (auto res : expectedResources) {
        CHECK(p1.getResourceCount(res) == 1);
    }
}


/*********************************************/
///             TESTS FOR CARDS             ///
/*********************************************/

TEST_CASE("VictoryPointCardUse") {
    Player player("Tom");
    vector<Player*> allPlayers{&player};
    Board& board = Board::getInstance();
    bool endTurn = false;
    VictoryPointCard card;

    int initialPoints = player.getPoints();
    card.activateCard(player, allPlayers, board, endTurn);

    CHECK(player.getPoints() == initialPoints + 1);
    CHECK(endTurn == true);
}

TEST_CASE("MonopolyCardUse") {
    Player player1("Pit"), player2("Dor");
    
    // Setup initial resources for player 2
    player2.addResource(ResourceType::WOOD, 5);
    
    // Assign a Monopoly card to player 1
    player1.setPromotionCardCount(PromotionType::MONOPOLY, 1);

    vector<Player*> allPlayers{&player1, &player2};
    Board& board = Board::getInstance();
    MonopolyCard card;
    bool endTurn;

    // Simulate player1 choosing to monopolize WOOD
    stringstream ss;
    ss << "1\n";  
    cin.rdbuf(ss.rdbuf());

    auto result = card.activateCard(player1, allPlayers, board, endTurn);
    CHECK(result == CardUseError::Success);

    CHECK(player1.getResourceCount(ResourceType::WOOD) == 5);       // Now pit has 5 woods
    CHECK(player2.getResourceCount(ResourceType::WOOD) == 0);       // Dor has no wood remaining
    cin.rdbuf(nullptr); 
}


/*********************************************/
///             TESTS FOR EDGE              ///
/*********************************************/

TEST_CASE("Edge operator< comparison") {
    Intersection i1 = Intersection::getIntersection(1);
    Intersection i2 = Intersection::getIntersection(2);
    Intersection i3 = Intersection::getIntersection(3);

    Edge edge1(i1, i2);
    Edge edge2(i2, i3);
    Edge edge3(i1, i3);
    Edge edge4(i2, i1);     

    CHECK(edge1 < edge3);
    CHECK(edge2 < edge1);
    CHECK_FALSE(edge1 < edge2);
    CHECK_FALSE(edge4 < edge1); 
}

TEST_CASE("Edge operator== comparison") {
    Intersection i1 = Intersection::getIntersection(1);
    Intersection i2 = Intersection::getIntersection(2);

    Edge edge1(i1, i2);
    Edge edge2(i2, i1); // Same as edge1 but reversed
    Edge edge3(i1, i1); // Self-loop (invalid but for test purpose)

    CHECK(edge1 == edge2);
    CHECK_FALSE(edge1 == edge3);
}

TEST_CASE("Edge involvesIntersection with map") {
    Intersection i1 = Intersection::getIntersection(1);
    Intersection i2 = Intersection::getIntersection(2);
    Intersection i3 = Intersection::getIntersection(3);

    map<int, Intersection> intersections = {
        {1, i1}, {2, i2}, {3, i3}
    };

    Edge edge(i1, i2);

    CHECK(edge.involvesIntersection(1, intersections));
    CHECK(edge.involvesIntersection(2, intersections));
    CHECK_FALSE(edge.involvesIntersection(3, intersections));
    CHECK_THROWS_AS(edge.involvesIntersection(4, intersections), out_of_range);
}

TEST_CASE("Edge involvesIntersection without map") {
    Intersection i1 = Intersection::getIntersection(1);
    Intersection i2 = Intersection::getIntersection(2);
    Intersection i3 = Intersection::getIntersection(3);

    Edge edge(i1, i2);

    CHECK(edge.involvesIntersection(1));
    CHECK(edge.involvesIntersection(2));
    CHECK_FALSE(edge.involvesIntersection(3));
}

TEST_CASE("Edge getters") {
    Intersection i1 = Intersection::getIntersection(1);
    Intersection i2 = Intersection::getIntersection(2);

    Edge edge(i1, i2);

    CHECK(edge.getIntersection1().id == 1);
    CHECK(edge.getIntersection2().id == 2);
    CHECK(edge.getId1() == 1);
    CHECK(edge.getId2() == 2);
}

TEST_CASE("Edge ostream operator<<") {
    Intersection i1 = Intersection::getIntersection(1);
    Intersection i2 = Intersection::getIntersection(2);

    Edge edge(i1, i2);

    stringstream ss;
    ss << edge;
    CHECK(ss.str() == "{ (10, 12) } to { (-10, -8) (1, 2) }");
}


/*********************************************/
///        TESTS FOR INTERSECTION           ///
/*********************************************/

TEST_CASE("Intersection Initialization") {

    Intersection::initialize();
    const auto& intersections = Intersection::getAllIntersections();
    CHECK(intersections.size() == 54); 

    // Check if specific intersections exist
    CHECK_NOTHROW(Intersection::getIntersection(1));
    CHECK_THROWS_AS(Intersection::getIntersection(100), out_of_range);
}

TEST_CASE("Intersection contains Vertex") {
    Vertex v1(10, 12);
    Intersection i1({v1}, 1);

    CHECK(i1.contains(v1));
    CHECK_FALSE(i1.contains(Vertex(0, 0)));
}


TEST_CASE("Intersection operator== comparison") {
    Intersection i1({Vertex(10, 12)}, 1);
    Intersection i2({Vertex(10, 12)}, 1);
    Intersection i3({Vertex(11, 12)}, 3);

    CHECK(i1 == i2);
    CHECK_FALSE(i1 == i3);
}

TEST_CASE("Intersection getIntersection") {
    Intersection::initialize();
    CHECK_NOTHROW(Intersection::getIntersection(1));
    CHECK_THROWS_AS(Intersection::getIntersection(100), out_of_range);
}

TEST_CASE("Intersection getAllIntersections") {
    Intersection::initialize();
    const auto& intersections = Intersection::getAllIntersections();
    CHECK(intersections.size() == 54);
}

TEST_CASE("Intersection ostream operator<<") {
    Intersection i1({Vertex(10, 12)}, 1);

    stringstream ss;
    ss << i1;
    CHECK(ss.str() == "{ (10, 12) }"); 
}

/*********************************************/
///             TESTS FOR TILE              ///
/*********************************************/

TEST_CASE("Tile Initialization") {
    Tile woodTile(ResourceType::WOOD, 8);
    CHECK(woodTile.getResourceType() == ResourceType::WOOD);
    CHECK(woodTile.getNumber() == 8);

    Tile oreTile(ResourceType::ORE, 5);
    CHECK(oreTile.getResourceType() == ResourceType::ORE);
    CHECK(oreTile.getNumber() == 5);

    DesertTile desertTile(0);
    CHECK(desertTile.getResourceType() == ResourceType::NONE);
    CHECK(desertTile.getNumber() == 0);
}

TEST_CASE("Tile Settlement Management") {
    Tile tile(ResourceType::BRICK, 4);
    Intersection inter1({Vertex(1, 2)}, 1);
    Intersection inter2({Vertex(2, 2)}, 2);

    CHECK_FALSE(tile.hasSettlement(inter1));

    tile.addSettlement(inter1);
    CHECK(tile.hasSettlement(inter1));
    CHECK(tile.getSettlements().size() == 1);

    tile.addSettlement(inter2);
    CHECK(tile.hasSettlement(inter2));
    CHECK(tile.getSettlements().size() == 2);
}

TEST_CASE("Tile Road Management") {
    Tile tile(ResourceType::WOOL, 6);
    Intersection inter1({Vertex(1, 2)}, 1);
    Intersection inter2({Vertex(2, 2)}, 2);
    Intersection inter3({Vertex(3, 2)}, 3);

    Edge edge1(inter1, inter2);
    Edge edge2(inter2, inter3);

    CHECK_FALSE(tile.hasRoad(edge1));

    tile.addRoad(edge1);
    CHECK(tile.hasRoad(edge1));
    CHECK(tile.getRoads().size() == 1);

    tile.addRoad(edge2);
    CHECK(tile.hasRoad(edge2));
    CHECK(tile.getRoads().size() == 2);
}

TEST_CASE("Tile Intersection Management") {
    Tile tile(ResourceType::GRAIN, 9);
    tile.addIntersection(1);
    tile.addIntersection(2);

    set<int> intersectionIDs = tile.getIntersectionIDs();
    CHECK(intersectionIDs.size() == 2);
    CHECK(intersectionIDs.count(1) == 1);
    CHECK(intersectionIDs.count(2) == 1);
}

TEST_CASE("DesertTile Initialization") {
    DesertTile desertTile(0);
    CHECK(desertTile.getResourceType() == ResourceType::NONE);
    CHECK(desertTile.getNumber() == 0);
}


/*********************************************/
///           TESTS FOR VERTEX              ///
/*********************************************/

TEST_CASE("Vertex Initialization") {
    Vertex v1(1, 2);
    CHECK(v1.xaxis == 1);
    CHECK(v1.yaxis == 2);

    Vertex v2(-1, -2);
    CHECK(v2.xaxis == -1);
    CHECK(v2.yaxis == -2);
}

TEST_CASE("Vertex Comparison Operators") {
    Vertex v1(1, 2);
    Vertex v2(2, 3);
    Vertex v3(1, 2);
    Vertex v4(1, 3);

    CHECK(v1 < v2);        // v1 is less than v2 because 1 < 2
    CHECK(v1 == v3);       // v1 is equal to v3 because (1, 2) == (1, 2)
    CHECK(v1 < v4);        // v1 is less than v4 because 2 < 3 (same x, different y)
    CHECK_FALSE(v2 < v1);  // v2 is not less than v1
    CHECK_FALSE(v1 == v2); // v1 is not equal to v2
}

TEST_CASE("Vertex Output Stream Operator") {
    Vertex v1(1, 2);
    Vertex v2(-1, -2);

    stringstream ss1;
    ss1 << v1;
    CHECK(ss1.str() == "(1, 2)");

    stringstream ss2;
    ss2 << v2;
    CHECK(ss2.str() == "(-1, -2)");
}