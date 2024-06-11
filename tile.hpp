// Email: origoldbsc@gmail.com

#ifndef TILE_HPP
#define TILE_HPP

#include "resources.hpp"
#include "intersection.hpp"
#include "edge.hpp"

using namespace std;
namespace ariel {


    /**
     * This class represents a tile in Settlers of Catan's board game.
     * A tile generates resources and can have settlements and roads.
     */
    class Tile {

        private:
            ResourceType resourceType;         // The type of resource this tile produces
            int number;                        // The number of the tile
            vector<Intersection> settlements;  // List of settlements built on this tile
            vector<Edge> roads;                // List of roads built on this tile
            set<int> intersectionIDs;          // Set of unique intersection IDs associated with this tile


        public:

            /**
             * @brief Constructs a new tile with a specified resource type and number.
             * @param resourceType The type of resource the tile will produce (default is NONE).
             * @param number The dice number for resource production.
             */
            Tile(ResourceType resourceType = ResourceType::NONE, int number = 0);


            //---------------------------//
            //      Accessor methods     // 
            //---------------------------//

            /**
             * @brief Returs the resource type of the tile.
             * @return The resource type of the tile.
             */
            virtual ResourceType getResourceType() const;

            /**
             * @brief Returs the dice number associated with the tile.
             * @return The dice number.
             */
            int getNumber() const;

            //---------------------------//
            //   Settlement management   // 
            //---------------------------//

            /**
             * @brief Checks if a specific intersection has a settlement.
             * @param in The intersection to check.
             * @return True if there is a settlement at the specified intersection; otherwise, false.
             */
            bool hasSettlement(const Intersection& in) const;

            /**
             * @brief Adds a settlement to a specific intersection on the tile.
             * @param in The intersection where the settlement will be added.
             */
            void addSettlement(const Intersection& in);


            //---------------------------//
            //       Road management     // 
            //---------------------------//

            /**
             * @brief Checks if a road exists on a specified edge of the tile.
             * @param edge The edge to check for a road.
             * @return True if there is a road on the edge; otherwise, false.
             */
            bool hasRoad(const Edge& edge) const;

            /**
             * @brief Adds a road to a specific edge of the tile.
             * @param edge The edge where the road will be added.
             */
            void addRoad(const Edge& edge);


            //---------------------------//
            //  Intersection management  // 
            //---------------------------//

            /**
             * @brief Adds an intersection ID to the set of intersection IDs.
             * @param intersectionID The intersection ID to add.
             */
            void addIntersection(int intersectionID);

            /**
             * @brief Retrieves the set of intersection IDs associated with the tile.
             * @return The set of intersection IDs.
             */
            set<int> getIntersectionIDs() const;

            

            //---------------------------//
            //          Getters          // 
            //---------------------------//

            /**
             * @brief Returns the list of settlements on the tile.
             * @return A constant reference to the vector of settlements.
             */
            const std::vector<Intersection>& getSettlements() const;

            /**
             * @brief Returns the list of roads on the tile.
             * @return A constant reference to the vector of roads.
             */
            const vector<Edge>& getRoads() const;
    };

    // SUB-CLASSE
    class DesertTile : public Tile {
        public:
            DesertTile(int number);
            ResourceType getResourceType() const override;
    };
}

#endif
