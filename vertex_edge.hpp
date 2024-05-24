#ifndef VERTEX_EDGE_HPP
#define VERTEX_EDGE_HPP

#include <utility>
#include <set>

namespace ariel {

    struct Vertex 
    {
        int x, y;

        // Constructor 
        Vertex(int x = 0, int y = 0) : x(x), y(y) {}    

        // Basic operators
        bool operator<(const Vertex& other) const 
        {
            return x < other.x || (x == other.x && y < other.y);
        }

        bool operator ==(const Vertex& other) const 
        {
            return x == other.x && y == other.y;
        }
    };

    struct Edge 
    {
        Vertex v1, v2;

        // Constructor 
        Edge(const Vertex& a, const Vertex& b) 
        {
            // Ensure the smaller vertex is always v1 for consistent comparison
            if (a < b) 
            {
                v1 = a;
                v2 = b;
            } 
            else 
            {
                v1 = b;
                v2 = a;
            }
        }

        // Basic operator
        bool operator<(const Edge& other) const 
        {
            return v1 < other.v1 || (v1 == other.v1 && v2 < other.v2);
        }
    };

    struct Intersection 
    {
        set<Vertex> vertices;       // A set of vertices that define this intersection

        // Constructor 
        Intersection(const std::initializer_list<Vertex>& verts) : vertices(verts) {}

        // Method to check if a given vertex is part of this intersection
        bool contains(const Vertex& vertex) const 
        {
            return vertices.find(vertex) != vertices.end();
        }

        // Less-than operator to store in ordered data structures
        bool operator<(const Intersection& other) const 
        {
            if (vertices.size() != other.vertices.size())
                return vertices.size() < other.vertices.size();
            return std::lexicographical_compare(vertices.begin(), vertices.end(),
                                                other.vertices.begin(), other.vertices.end());
        }
    };
}

#endif
