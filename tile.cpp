// tile.cpp
#include "tile.hpp"

namespace ariel {

    Tile::Tile(ResourceType type, int num) : resourceType(type), number(num), settlement(false), road(false) {}

    ResourceType Tile::getResourceType() const 
    {
        return resourceType;
    }

    int Tile::getNumber() const 
    {
        return number;
    }

    bool Tile::hasSettlement() const 
    {
        return settlement;
    }

    void Tile::setSettlement(bool status) 
    {
        settlement = status;
    }

    bool Tile::hasRoad() const 
    {
        return road;
    }

    void Tile::setRoad(bool status) 
    {
        road = status;
    }


    // SUB-CLASSES

    // ForestTile
    ForestTile::ForestTile(int number) : Tile(ResourceType::WOOD, number) {}

    // HillsTile
    HillsTile::HillsTile(int number) : Tile(ResourceType::BRICK, number) {}

    // PastureTile
    PastureTile::PastureTile(int number) : Tile(ResourceType::WOOL, number) {}

    // AgriculturalTile
    AgriculturalTile::AgriculturalTile(int number) : Tile(ResourceType::GRAIN, number) {}

    // MountainsTile
    MountainsTile::MountainsTile(int number) : Tile(ResourceType::ORE, number) {}

    // DesertTile
    DesertTile::DesertTile(int number) : Tile(ResourceType::NONE, number) {}

    ResourceType DesertTile::getResourceType() const 
    {
        return ResourceType::NONE;
    }

}
