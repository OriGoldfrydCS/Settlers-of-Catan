#ifndef VERTEX_EDGE_HPP
#define VERTEX_EDGE_HPP

#include <utility>

namespace ariel {

    struct Vertex {
        int x, y;

        Vertex(int x = 0, int y = 0) : x(x), y(y) {}  // Default constructor with parameters

        // Less-than operator for using Vertex in ordered data structures like std::map
        bool operator<(const Vertex& other) const {
            return x < other.x || (x == other.x && y < other.y);
        }

        bool operator ==(const Vertex& other) const {
            return x == other.x && y == other.y;
        }
    };

    struct Edge {
        Vertex v1, v2;

        // Constructor to ensure the smaller vertex is always v1 for consistent comparison
        Edge(const Vertex& a, const Vertex& b) {
            if (a < b) {
                v1 = a;
                v2 = b;
            } else {
                v1 = b;
                v2 = a;
            }
        }

        // Less-than operator for using Edge in ordered data structures like std::map
        bool operator<(const Edge& other) const {
            return v1 < other.v1 || (v1 == other.v1 && v2 < other.v2);
        }
    };
}

#endif
