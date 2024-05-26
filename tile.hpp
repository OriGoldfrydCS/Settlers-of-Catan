#ifndef TILE_HPP
#define TILE_HPP

#include <string>
#include <vector>
#include "resource_type.hpp"
#include "vertex_edge.hpp"
#include "intersection.hpp"
#include "edge.hpp"

using namespace std;
namespace ariel {

    class Tile {
    public:

        private:
            ResourceType resourceType;         // The resource that the tile can produce
            int number;                        // The number of the tile
            vector<Intersection> settlements;  // Stores vertices where settlements are located
            vector<Edge> roads;                // Stores edges where roads are built
            set<int> intersectionIDs;          // Holds intersection IDs related to this tile


    public:
        // Constructor
        Tile(ResourceType resourceType = ResourceType::NONE, int number = 0);

        // Destructor
        virtual ~Tile() = default;

        // Accessor methods
        virtual ResourceType getResourceType() const;
        int getNumber() const;

        // Settlement management
        bool hasSettlement(const Intersection& in) const;
        void addSettlement(const Intersection& in);

        // Road management
        bool hasRoad(const Edge& edge) const;
        void addRoad(const Edge& edge);

        void addIntersection(int intersectionID) {
            intersectionIDs.insert(intersectionID);
        }

        vector<int> getIntersectionIDs() const;

        // Getter methods
        const std::vector<Intersection>& getSettlements() const;
        const std::vector<Edge>& getRoads() const;

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
