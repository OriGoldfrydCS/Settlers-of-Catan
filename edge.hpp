// Email: origoldbsc@gmail.com

#ifndef EDGE_HPP
#define EDGE_HPP

#include <map>
#include "intersection.hpp"

using namespace std;
namespace ariel {
    
    class Edge {
        
        private:

            Intersection i1, i2;        // The two intersections connected by this edge

        public:

            /**
             * @brief Constructs an edge between two intersections.
             * 
             * @param a The first intersection.
             * @param b The second intersection.
             */
            Edge(const Intersection& a, const Intersection& b) 
            {
                if (a.getId() < b.getId()) 
                {
                    i1 = a;
                    i2 = b;
                } 
                else 
                {
                    i1 = b;
                    i2 = a;
                }
            }


            /**
             * @brief Compares two edges for ordering.
             * @param other The other edge to compare with.
             * @return bool True if this edge is less than the other edge.
             */
            bool operator<(const Edge& other) const;
            
            
            /**
             * @brief Compares two edges for equality.
             * @param other The other edge to compare with.
             * @return bool True if the edges are equal.
             */
            bool operator==(const Edge& other) const;
            
            
             /**
             * @brief Checks if the edge involves a specific intersection by its ID using a map of intersections.
             * @param intersectionID The ID of the intersection to check.
             * @param intersections The map of all intersections.
             * @return bool True if the edge involves the specified intersection.
             */
            bool involvesIntersection(int intersectionID, const map<int, Intersection>& intersections) const;


             /**
             * @brief Checks if the edge involves a specific intersection by its ID.
             * @param intersectionID The ID of the intersection to check.
             * @return bool True if the edge involves the specified intersection.
             */
            bool involvesIntersection(int intersectionID) const;


            /**
             * @brief Gets the first intersection.
             * @return The first intersection.
             */
            const Intersection& getIntersection1() const;


            /**
             * @brief Gets the second intersection.
             * 
             * @return The second intersection.
             */
            const Intersection& getIntersection2() const;


            /**
             * @brief Gets the ID of the first intersection.
             * @return int The ID of the first intersection.
             */
            int getId1() const;


            /**
             * @brief Gets the ID of the second intersection.
             * @return int The ID of the second intersection.
             */
            int getId2() const;


            /**
             * @brief Outputs the edge details to an output stream.
             * 
             * @param os The output stream to write to.
             * @param edge The edge to print.
             * @return std::ostream& The output stream with the edge details.
             */
            friend ostream& operator<<(ostream& os, const Edge& edge);
        }; 
}

#endif