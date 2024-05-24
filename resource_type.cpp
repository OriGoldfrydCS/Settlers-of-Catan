#include "resource_type.hpp"

string ariel::resourceTypeToString(ariel::ResourceType resource) 
{
    switch (resource) {
        case ariel::WOOD:   return "WOOD";
        case ariel::BRICK:  return "BRICK";
        case ariel::WOOL:   return "WOOL";
        case ariel::GRAIN:  return "GRAIN";
        case ariel::ORE:    return "ORE";
        case ariel::NONE:   return "DESERT";
        default:            return "UNKNOWN";
    }
}