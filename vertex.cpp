#include "vertex.hpp"

namespace ariel {
    
    bool Vertex::operator<(const Vertex& other) const {
        return x < other.x || (x == other.x && y < other.y);
    }

    bool Vertex::operator==(const Vertex& other) const {
        return x == other.x && y == other.y;
    }

    std::ostream& operator<<(std::ostream& os, const Vertex& vertex) {
        os << "(" << vertex.x << ", " << vertex.y << ")";
        return os;
    }
}