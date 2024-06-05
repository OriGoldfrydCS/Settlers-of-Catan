#include "resources.hpp"

/**
 * @brief Converts a ResourceType enum value to its corresponding string representation. 
 * @param resource The resource type enum to convert.
 * @return A string representing the resource type.
 *         Returns "DESERT" for ResourceType::NONE to represent a non-resource producing tile.
 *         Returns "UNKNOWN" for any undefined ResourceType values.
 */
string ariel::resourceTypeToString(ariel::ResourceType resource) 
{
    switch (resource) 
    {
        case ariel::WOOD:   return "WOOD";
        case ariel::BRICK:  return "BRICK";
        case ariel::WOOL:   return "WOOL";
        case ariel::GRAIN:  return "GRAIN";
        case ariel::ORE:    return "ORE";
        case ariel::NONE:   return "DESERT";
        default:            return "UNKNOWN";
    }
}