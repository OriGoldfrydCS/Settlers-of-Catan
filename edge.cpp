// Email: origoldbsc@gmail.com

#include "edge.hpp"

using namespace std;
namespace ariel {
    

     /**
     * @brief Compares two edges for ordering.
     * @param other The other edge to compare with.
     * @return bool True if this edge is less than the other edge.
     */
    bool Edge::operator<(const Edge& other) const 
    {
        return i1 < other.i1 || (i1 == other.i1 && i2 < other.i2);
    }


     /**
     * @brief Compares two edges for equality.
     * @param other The other edge to compare with.
     * @return bool True if the edges are equal.
     */
    bool Edge::operator==(const Edge& other) const 
    {
        return (i1 == other.i1 && i2 == other.i2) || (i1 == other.i2 && i2 == other.i1);
    }


    /**
     * @brief Checks if the edge involves a specific intersection by its ID using a map of intersections.
     * @param intersectionID The ID of the intersection to check.
     * @param intersections The map of all intersections.
     * @return bool True if the edge involves the specified intersection.
     * @throws out_of_range if the intersection ID is not found in the map.
     */
    bool Edge::involvesIntersection(int intersectionID, const map<int, Intersection>& intersections) const 
    {
        auto it1 = intersections.find(intersectionID);
        if (it1 == intersections.end()) {
            throw out_of_range("Invalid intersection ID provided");
        }
        return (i1 == it1->second || i2 == it1->second);
    }


    /**
     * @brief Checks if the edge involves a specific intersection by its ID.
     * @param intersectionID The ID of the intersection to check.
     * @return bool True if the edge involves the specified intersection.
     */
    bool Edge::involvesIntersection(int intersectionID) const 
    {
        // Compare using the id attribute of the Intersection objects
        return (i1.id == intersectionID || i2.id == intersectionID);
    }


    /**
     * @brief Gets the first intersection.
     * @return The first intersection.
     */
    const Intersection& Edge::getIntersection1() const 
    { 
        return i1; 
    }



    /**
     * @brief Gets the second intersection.
     * 
     * @return The second intersection.
     */
    const Intersection& Edge::getIntersection2() const 
    { 
        return i2; 
    }


    /**
     * @brief Gets the ID of the first intersection.
     * @return int The ID of the first intersection.
     */
    int Edge::getId1() const 
    {   
        return i1.id; 
    }


    /**
     * @brief Gets the ID of the second intersection.
     * @return int The ID of the second intersection.
     */
    int Edge::getId2() const 
    { 
        return i2.id; 
    }


    /**
     * @brief Outputs the edge details to an output stream.
     * @param os The output stream to write to.
     * @param edge The edge to print.
     * @return ostream& The output stream with the edge details.
     */
    ostream& operator<<(ostream& os, const Edge& edge) 
    {
        os << edge.i1 << " to " << edge.i2;
        return os;
    }
    
}