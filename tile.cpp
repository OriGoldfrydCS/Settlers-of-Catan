// Email: origoldbsc@gmail.com

#include "tile.hpp"
#include <vector>

using namespace std;
namespace ariel {


    /**
     * @brief Constructs a new tile with a specified resource type and number.
     * @param resourceType The type of resource the tile will produce (default is NONE).
     * @param number The dice number for resource production.
     */
    Tile::Tile(ResourceType type, int num) : resourceType(type), number(num) {}


    /**
     * @brief Returs the resource type of the tile.
     * @return The resource type of the tile.
     */
    ResourceType Tile::getResourceType() const 
    {
        return resourceType;
    }


    /**
     * @brief Returs the dice number associated with the tile.
     * @return The dice number.
     */
    int Tile::getNumber() const 
    {
        return number;
    }


    /**
     * @brief Checks if a specific intersection has a settlement.
     * @param in The intersection to check.
     * @return True if there is a settlement at the specified intersection; otherwise, false.
     */
    bool Tile::hasSettlement(const Intersection& inter) const 
    {
        return find(settlements.begin(), settlements.end(), inter) != settlements.end();
    }


    /**
     * @brief Adds a settlement to a specific intersection on the tile.
     * @param in The intersection where the settlement will be added.
     */
    void Tile::addSettlement(const Intersection& inter) 
    {
        if (!hasSettlement(inter)) 
        {
            settlements.push_back(inter);
        }
    }


    /**
     * @brief Checks if a road exists on a specified edge of the tile.
     * @param edge The edge to check for a road.
     * @return True if there is a road on the edge; otherwise, false.
     */
    bool Tile::hasRoad(const Edge& edge) const 
    {
        return find(roads.begin(), roads.end(), edge) != roads.end();
    }


    /**
     * @brief Adds a road to a specific edge of the tile.
     * @param edge The edge where the road will be added.
     */
    void Tile::addRoad(const Edge& edge) 
    {
        if (!hasRoad(edge)) {
            roads.push_back(edge);
        }
    }


    /**
     * @brief Adds an intersection ID to the set of intersection IDs.
     * @param intersectionID The intersection ID to add.
     */
    void Tile::addIntersection(int intersectionID) 
    {
        intersectionIDs.insert(intersectionID);
    }


    /**
     * @brief Retrieves the set of intersection IDs associated with the tile.
     * @return The set of intersection IDs.
     */
    set<int> Tile::getIntersectionIDs() const 
    {
        return set<int>(intersectionIDs.begin(), intersectionIDs.end());
    }


    /**
     * @brief Returns the list of settlements on the tile.
     * @return A constant reference to the vector of settlements.
     */    
    const vector<Intersection>& Tile::getSettlements() const 
    {
        return settlements;
    }


    /**
     * @brief Returns the list of roads on the tile.
     * @return A constant reference to the vector of roads.
     */
    const vector<Edge>& Tile::getRoads() const 
    {
        return roads;
    }


    // SUB-CLASSE (for unique desert tile)
    DesertTile::DesertTile(int number) : Tile(ResourceType::NONE, number) {}

    ResourceType DesertTile::getResourceType() const 
    {
        return ResourceType::NONE;
    }

}
