#ifndef TILE_HPP
#define TILE_HPP

#include <string>
#include "resource_type.hpp"

namespace ariel {

    class Tile {
    public:
        ResourceType resourceType;
        int number;

        // Constructor
        Tile(ResourceType resourceType = ResourceType::NONE, int number = 0);

        // Method declarations
        virtual ResourceType getResourceType() const;
        int getNumber() const;
    };

    class ForestTile : public Tile {
        public:
            ForestTile(int number) : Tile(ResourceType::WOOD, number) {}
        };

    class HillsTile : public Tile {
        public:
            HillsTile(int number) : Tile(ResourceType::BRICK, number) {}
    };

    class PastureTile : public Tile {
        public:
            PastureTile(int number) : Tile(ResourceType::WOOL, number) {}
    };

    class AgriculturalTile : public Tile {
        public:
            AgriculturalTile(int number) : Tile(ResourceType::GRAIN, number) {}
    };

    class MountainsTile : public Tile {
        public:
            MountainsTile(int number) : Tile(ResourceType::ORE, number) {}
    };

    class DesertTile : public Tile {
        public:
            DesertTile(int number) : Tile(ResourceType::NONE, number) {}
            ResourceType getResourceType() const { return ResourceType::NONE; }
    };

}

#endif
