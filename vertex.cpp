// Email: origoldbsc@gmail.com

#include "vertex.hpp"

using namespace std;
namespace ariel {
    

   /**
     * @brief Comparison operator to determine if this vertex is less than another based on its coordinates.
     * @param other The vertex to compare to.
     * @return True if this vertex is less than the other; otherwise, false.
     */
    bool Vertex::operator<(const Vertex& other) const 
    {
        return xaxis < other.xaxis || (xaxis == other.xaxis && yaxis < other.yaxis);
    }


    /**
     * @brief Equality operator to determine if two vertices have the same coordinates.
     * @param other The vertex to compare against.
     * @return True if both vertices have the same coordinates; otherwise, false.
     */
    bool Vertex::operator==(const Vertex& other) const 
    {
        return xaxis == other.xaxis && yaxis == other.yaxis;
    }


    /**
     * @brief Overload the output stream operator to provide a way to print the Vertex.
     * @param os The output stream.
     * @param vertex The vertex to be output.
     * @return A reference to the output stream.
     */
    ostream& operator<<(ostream& os, const Vertex& vertex) 
    {
        os << "(" << vertex.xaxis << ", " << vertex.yaxis << ")";
        return os;
    };
}