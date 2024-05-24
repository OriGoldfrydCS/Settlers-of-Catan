#ifndef RESOURCE_TYPE_HPP
#define RESOURCE_TYPE_HPP
#include <string>

using namespace std;
namespace ariel {

    enum ResourceType { WOOD, BRICK, WOOL, GRAIN, ORE , NONE};

    string resourceTypeToString(ResourceType resource);

}

#endif
