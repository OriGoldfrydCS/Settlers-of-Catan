// Email: origoldbsc@gmail.com

#ifndef VERTEX_HPP
#define VERTEX_HPP

#include <iostream>

using namespace std;
namespace ariel {
    
    /**
     * This class represents a 2D point with integer coordinates.
     * A vertex is used to identify a certain tiles on the game board.
     */
    class Vertex {
        
        public:

            int xaxis, yaxis;      // Coordinates of the vertex

            /**
             * @brief Constructor that initializes the vertex with given coordinates.
             * @param x The x-coordinate of the vertex.
             * @param y The y-coordinate of the vertex.
             */
            Vertex(int x, int y) : xaxis(x), yaxis(y) {}

            /**
             * @brief Comparison operator to determine if this vertex is less than another based on its coordinates.
             * @param other The vertex to compare to.
             * @return True if this vertex is less than the other; otherwise, false.
             */
            bool operator<(const Vertex& other) const;


             /**
             * @brief Equality operator to determine if two vertices have the same coordinates.
             * @param other The vertex to compare against.
             * @return True if both vertices have the same coordinates; otherwise, false.
             */
            bool operator==(const Vertex& other) const;


            /**
             * @brief Overload the output stream operator to provide a way to print the Vertex.
             * @param os The output stream.
             * @param vertex The vertex to be output.
             * @return A reference to the output stream.
             */
            friend ostream& operator<<(ostream& os, const Vertex& vertex);
        };
    }

#endif