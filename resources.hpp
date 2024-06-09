// Email: origoldbsc@gmail.com

#ifndef RESOURCE_TYPE_HPP
#define RESOURCE_TYPE_HPP

#include <string>

using namespace std;
namespace ariel {
    
    /**
     * Enumerates the types of resources available in the game.
     * These resources are used to build and progress within the game.
     */
    enum ResourceType { WOOD, BRICK, WOOL, GRAIN, ORE , NONE};


    /**
     * @brief Converts a ResourceType enum value to its corresponding string representation. 
     * @param resource The resource type enum to convert.
     * @return A string representing the resource type.
     *         Returns "DESERT" for ResourceType::NONE to represent a non-resource producing tile.
     *         Returns "UNKNOWN" for any undefined ResourceType values.
     */
    string resourceTypeToString(ResourceType resource);

};

#endif
