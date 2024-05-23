#include "tile.hpp"

namespace ariel {

    // Constructor definition
    Tile::Tile(ResourceType resourceType, int number) : resourceType(resourceType), number(number) {}

    ResourceType Tile::getResourceType() const 
    {
        return resourceType;
    }

    int Tile::getNumber() const 
    {
        return number;
    }
}