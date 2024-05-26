#ifndef EDGE_HPP
#define EDGE_HPP

#include <map>
#include "intersection.hpp"

namespace ariel {
    
    class Edge {
        public:
            Intersection i1, i2;

            Edge(const Intersection& a, const Intersection& b) : i1(a < b ? a : b), i2(a < b ? b : a) {}

            bool operator<(const Edge& other) const;
            bool operator==(const Edge& other) const;
            bool involvesIntersection(int intersectionID, const std::map<int, Intersection>& intersections) const;

            friend std::ostream& operator<<(std::ostream& os, const Edge& edge);
        };
}

#endif