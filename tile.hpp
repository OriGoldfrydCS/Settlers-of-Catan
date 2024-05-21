#ifndef TILE_HPP
#define TILE_HPP

#include <string>

using namespace std;
namespace ariel {

    class Tile 
    {
        protected:
            string resourceType;
            int number;

        public:
            Tile(std::string resourceType, int number);
            virtual string getResourceType() const;
            int getNumber() const;
            
    };

    class ForestTile : public Tile {
        public:
            ForestTile(int number);
    };

    class HillsTile : public Tile 
    {
        public:
            HillsTile(int number);
    };

    class PastureTile : public Tile 
    {
        public:
            PastureTile(int number);
    };

    class AgriculturalTile : public Tile 
    {
        public:
            AgriculturalTile(int number);
    };

    class MountainsTile : public Tile 
    {
        public:
            MountainsTile(int number);
    };

    class DesertTile : public Tile 
    {
        public:
            DesertTile(int number);
            string getResourceType() const override;      // We Override the function to indicate no resources
    };

}

#endif
