// tile.cpp
#include "tile.hpp"
#include <algorithm>

using namespace std;
namespace ariel {

    Tile::Tile(ResourceType type, int num) : resourceType(type), number(num) {}

    ResourceType Tile::getResourceType() const 
    {
        return resourceType;
    }

    int Tile::getNumber() const 
    {
        return number;
    }

    bool Tile::hasSettlement(const Vertex& location) const 
    {
        return find(settlements.begin(), settlements.end(), location) != settlements.end();
    }

    void Tile::addSettlement(const Vertex& location) 
    {
        if (!hasSettlement(location)) 
        {
            settlements.push_back(location);
        }
    }

    bool Tile::hasRoad(const Edge& edge) const 
    {
        return find(roads.begin(), roads.end(), edge) != roads.end();
    }

    void Tile::addRoad(const Edge& edge) 
    {
        if (!hasRoad(edge)) {
            roads.push_back(edge);
        }
    }


    // SUB-CLASSES

    // Specific Tile type implementations
    ForestTile::ForestTile(int number) : Tile(ResourceType::WOOD, number) {}
    HillsTile::HillsTile(int number) : Tile(ResourceType::BRICK, number) {}
    PastureTile::PastureTile(int number) : Tile(ResourceType::WOOL, number) {}
    AgriculturalTile::AgriculturalTile(int number) : Tile(ResourceType::GRAIN, number) {}
    MountainsTile::MountainsTile(int number) : Tile(ResourceType::ORE, number) {}
    DesertTile::DesertTile(int number) : Tile(ResourceType::NONE, number) {}

    ResourceType DesertTile::getResourceType() const 
    {
        return ResourceType::NONE;
    }

}
