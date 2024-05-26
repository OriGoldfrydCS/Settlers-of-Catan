#include "intersection.hpp"

using namespace std;
namespace ariel {

    std::map<int, Intersection> Intersection::intersections;

    void Intersection::initialize() 
    {
        if (intersections.empty()) 
        {
            initializeIntersections();
        }
    }

    bool Intersection::contains(const Vertex& vertex) const 
    {
        return vertices.find(vertex) != vertices.end();
    }

    bool Intersection::operator<(const Intersection& other) const 
    {
        return std::lexicographical_compare(vertices.begin(), vertices.end(),
                                            other.vertices.begin(), other.vertices.end());
    }

    bool Intersection::operator==(const Intersection& other) const 
    {
        return vertices == other.vertices;
    }

    ostream& operator<<(std::ostream& os, const Intersection& intersection) 
    {
        os << "{ ";
        for (const auto& vertex : intersection.vertices) {
            os << vertex << " ";
        }
        os << "}";
        return os;
    }

    const std::map<int, Intersection>& Intersection::getAllIntersections() 
    {
        if (intersections.empty()) 
        {
            initializeIntersections();
        }
        return intersections;
    }

    Intersection Intersection::getIntersection(int intersectionID) 
    {
        const auto& allIntersections = getAllIntersections();
        auto it = allIntersections.find(intersectionID);
        if (it == allIntersections.end()) {
            throw std::out_of_range("Invalid intersection ID");
        }
        return it->second;
    }

    void Intersection::initializeIntersections() 
    {
        intersections = {
            {1, Intersection({Vertex(0, 2), Vertex(1, 2)}, 1)},
            {2, Intersection({Vertex(1, 2), Vertex(2, 2)}, 2)},
            {3, Intersection({Vertex(-1, 1), Vertex(0, 2)}, 3)},
            {4, Intersection({Vertex(-1, 1), Vertex(0, 1), Vertex(0, 2)}, 4)},
            {5, Intersection({Vertex(0, 2), Vertex(0, 1), Vertex(1, 2)}, 5)},
            {6, Intersection({Vertex(0, 1), Vertex(1, 2), Vertex(1, 1)}, 6)},
            {7, Intersection({Vertex(1, 2), Vertex(1, 1), Vertex(2, 2)}, 7)},
            {8, Intersection({Vertex(1, 1), Vertex(2, 2), Vertex(2, 1)}, 8)},
            {9, Intersection({Vertex(2, 2), Vertex(2, 1)}, 9)},
            {10, Intersection({Vertex(-2, 0), Vertex(-1, 1)}, 10)},
            {11, Intersection({Vertex(-2, 0), Vertex(-1, 1), Vertex(-1, 0)}, 11)},
            {12, Intersection({Vertex(-1, 1), Vertex(-1, 0), Vertex(0, 1)}, 12)},
            {13, Intersection({Vertex(-1, 0), Vertex(0, 1), Vertex(0, 0)}, 13)},
            {14, Intersection({Vertex(0, 1), Vertex(0, 0), Vertex(1, 1)}, 14)},
            {15, Intersection({Vertex(0, 0), Vertex(1, 1), Vertex(1, 0)}, 15)},
            {16, Intersection({Vertex(1, 1), Vertex(1, 0), Vertex(2, 1)}, 16)},
            {17, Intersection({Vertex(1, 0), Vertex(2, 1), Vertex(2, 0)}, 17)},
            {18, Intersection({Vertex(2, 0), Vertex(2, 1)}, 18)},
            {19, Intersection({Vertex(-2, -1), Vertex(-2, 0)}, 19)},
            {20, Intersection({Vertex(-2, 0), Vertex(-2, -1), Vertex(-1, 0)}, 20)},
            {21, Intersection({Vertex(-2, -1), Vertex(-1, 0), Vertex(-1, -1)}, 21)},
            {22, Intersection({Vertex(-1, 0), Vertex(-1, -1), Vertex(0, 0)}, 22)},
            {23, Intersection({Vertex(-1, -1), Vertex(0, 0), Vertex(0, -1)}, 23)},
            {24, Intersection({Vertex(0, 0), Vertex(0, -1), Vertex(1, 0)}, 24)},
            {25, Intersection({Vertex(-1, 0), Vertex(1, 0), Vertex(1, -1)}, 25)},
            {26, Intersection({Vertex(1, 0), Vertex(1, -1), Vertex(2, 0)}, 26)},
            {27, Intersection({Vertex(2, 0), Vertex(1, -1)}, 27)},
            {28, Intersection({Vertex(-2, 2), Vertex(-2, -1)}, 28)},
            {29, Intersection({Vertex(-2, -1), Vertex(-2, -2), Vertex(-1, -1)}, 29)},
            {30, Intersection({Vertex(-2, -2), Vertex(-1, -1), Vertex(-1, -2)}, 30)},
            {31, Intersection({Vertex(-1, -1), Vertex(-1, -2), Vertex(0, -1)}, 31)},
            {32, Intersection({Vertex(-1, -2), Vertex(0, -1), Vertex(0, -2)}, 32)},
            {33, Intersection({Vertex(0, -1), Vertex(0, -2), Vertex(1, -1)}, 33)},
            {34, Intersection({Vertex(1, -1), Vertex(0, -2)}, 34)},
            {35, Intersection({Vertex(-1, -2), Vertex(-2, -2)}, 35)},
            {36, Intersection({Vertex(0, -2), Vertex(-1, -2)}, 36)}
        };
    }
}