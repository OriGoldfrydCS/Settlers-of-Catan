// Email: origoldbsc@gmail.com

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
    

    /**
     * @brief This class represents an intersection defined by a set of vertices.
     */
    class Intersection {
        
        private:
            
            /**
             * @brief Static map to store all intersections.
             */
            static map<int, Intersection> intersections;
            
            /**
             * @brief Initializes the intersections map with predefined intersections.
             * This function populates the intersections map with predefined intersections,
             * each defined by a set of vertices and a unique ID.
             */
            static void initializeIntersections();
        
        public:
            set<Vertex> vertices;      // A set of vertices that define this intersection
            int id;                    // Intersection ID

            /**
             * @brief Default constructor.
             */
            Intersection() {}


            /**
             * @brief Constructs an intersection with a set of vertices and an ID.
             * @param verts A set of vertices that define the intersection.
             * @param id The unique identifier for the intersection.
             */
            Intersection(const set<Vertex>& verts, int id) : vertices(verts), id(id) {}
            

            /**
             * @brief Initializes intersections data.
             */
            static void initialize(); 


            /**
             * @brief Checks if the intersection contains a specific vertex.
             * @param vertex The vertex to check.
             * @return bool True if the vertex is part of the intersection, false otherwise.
             */
            bool contains(const Vertex& vertex) const;
            
            
            /**
             * @brief Compares two intersections for ordering based on their vertices.
             * @param other The other intersection to compare with.
             * @return bool True if this intersection's vertices are lexicographically less than the other's.
             */
            bool operator<(const Intersection& other) const;
            
            
            /**
             * @brief Compares two intersections for equality based on their vertices.
             * @param other The other intersection to compare with.
             * @return bool True if the intersections have the same vertices, false otherwise.
             */
            bool operator==(const Intersection& other) const;


             /**
             * @brief Outputs the intersection details to an output stream.
             * @param os The output stream to write to.
             * @param intersection The intersection to print.
             * @return ostream& The output stream with the intersection details.
             */
            friend ostream& operator<<(ostream& os, const Intersection& intersection);


            /**
             * @brief Returns all intersections.
             * @return A reference to the map of all intersections.
             */
            static const map<int, Intersection>& getAllIntersections();
            
            
            /**
             * @brief Returns a specific intersection by its ID.
             * @param intersectionID The ID of the intersection to retrieve.
             * @return Intersection The intersection with the specified ID.
             * @throws out_of_range if the intersection ID is not found.
             */
            static Intersection getIntersection(int intersectionID);
        };
    }

#endif