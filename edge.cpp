#include "edge.hpp"

namespace ariel {

    bool Edge::operator<(const Edge& other) const {
        return i1 < other.i1 || (i1 == other.i1 && i2 < other.i2);
    }

    bool Edge::operator==(const Edge& other) const {
        return (i1 == other.i1 && i2 == other.i2) || (i1 == other.i2 && i2 == other.i1);
    }

    bool Edge::involvesIntersection(int intersectionID, const std::map<int, Intersection>& intersections) const {
        auto it1 = intersections.find(intersectionID);
        if (it1 == intersections.end()) {
            throw std::out_of_range("Invalid intersection ID provided");
        }
        return (i1 == it1->second || i2 == it1->second);
    }

    std::ostream& operator<<(std::ostream& os, const Edge& edge) {
        os << edge.i1 << " to " << edge.i2;
        return os;
    }
}