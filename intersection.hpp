#ifndef INTERSECTION_HPP
#define INTERSECTION_HPP

#include <set>
#include <algorithm>
#include <initializer_list>
#include <iostream>
#include <map>
#include "vertex.hpp"

using namespace std;
namespace ariel {
    
    class Intersection {
        public:
            std::set<Vertex> vertices;      // A set of vertices that define this intersection
            int id;                         // Intersection ID

            Intersection() {}
            Intersection(const std::set<Vertex>& verts, int id) : vertices(verts), id(id) {}
            
            static void initialize(); 

            bool contains(const Vertex& vertex) const;
            bool operator<(const Intersection& other) const;
            bool operator==(const Intersection& other) const;

            friend ostream& operator<<(std::ostream& os, const Intersection& intersection);

            static const map<int, Intersection>& getAllIntersections();
            static Intersection getIntersection(int intersectionID);

        private:
            static map<int, Intersection> intersections;
            static void initializeIntersections();

        };
    }

#endif