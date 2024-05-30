#ifndef EDGE_HPP
#define EDGE_HPP

#include <map>
#include "intersection.hpp"

namespace ariel {
    
    class Edge {
        public:
            Intersection i1, i2;

            Edge(const Intersection& a, const Intersection& b) {
                if (a.id < b.id) {
                    i1 = a;
                    i2 = b;
                } else {
                    i1 = b;
                    i2 = a;
                }
            }

            bool operator<(const Edge& other) const;
            bool operator==(const Edge& other) const;
            bool involvesIntersection(int intersectionID, const std::map<int, Intersection>& intersections) const;

            bool involvesIntersection(int intersectionID) const;

            int getId1() const { return i1.id; }
            int getId2() const { return i2.id; }
            friend std::ostream& operator<<(std::ostream& os, const Edge& edge);
        };
}

#endif