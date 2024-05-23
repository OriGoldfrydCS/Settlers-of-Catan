#ifndef TILE_HPP
#define TILE_HPP

#include <string>
#include "resource_type.hpp"

namespace ariel {

    class Tile {
    public:

        private:
            ResourceType resourceType;  // The resource that the tile can produce
            int number;                 // The number of the tile
            bool settlement;            // Tracks if there's a settlement on this tile
            bool road;                  // Tracks if there's a road on this tile

    public:
        // Constructor
        Tile(ResourceType resourceType = ResourceType::NONE, int number = 0);

        // Destructor
        virtual ~Tile() = default;

        // Accessor methods
        virtual ResourceType getResourceType() const;
        int getNumber() const;

        // Settlement management
        bool hasSettlement() const;
        void setSettlement(bool status);

        // Road management
        bool hasRoad() const;
        void setRoad(bool status);
    };



    // SUB-CLASSES

    class ForestTile : public Tile {
        public:
            ForestTile(int number);
    };

    class HillsTile : public Tile {
        public:
            HillsTile(int number);
    };

    class PastureTile : public Tile {
        public:
            PastureTile(int number);
    };

    class AgriculturalTile : public Tile {
        public:
            AgriculturalTile(int number);
    };

    class MountainsTile : public Tile {
        public:
            MountainsTile(int number);
    };

    class DesertTile : public Tile {
        public:
            DesertTile(int number);
            ResourceType getResourceType() const override;
    };

}

#endif
