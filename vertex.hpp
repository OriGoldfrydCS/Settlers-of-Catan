#ifndef VERTEX_HPP
#define VERTEX_HPP

#include <iostream>

namespace ariel {
    
    class Vertex {
        public:
            int x, y;

            Vertex(int x = 0, int y = 0) : x(x), y(y) {}

            bool operator<(const Vertex& other) const;
            bool operator==(const Vertex& other) const;

            friend std::ostream& operator<<(std::ostream& os, const Vertex& vertex);
        };
    }

#endif