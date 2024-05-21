#include "tile.hpp"

namespace ariel {

    Tile::Tile(string resourceType, int number) : resourceType(resourceType), number(number) {}

    string Tile::getResourceType() const 
    {
        return this->resourceType;
    }

    int Tile::getNumber() const 
    {
        return this->number;
    }

    ForestTile::ForestTile(int number) : Tile("Forest", number) {}

    HillsTile::HillsTile(int number) : Tile("Hills", number) {}

    PastureTile::PastureTile(int number) : Tile("Pasture", number) {}

    AgriculturalTile::AgriculturalTile(int number) : Tile("Agricultural", number) {}

    MountainsTile::MountainsTile(int number) : Tile("Mountains", number) {}

    DesertTile::DesertTile(int number) : Tile("Desert", number) {}

    string DesertTile::getResourceType() const 
    {
        return "None";
    }
}