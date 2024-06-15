// Email: origoldbsc@gmail.com

#include "intersection.hpp"
#include "tile.hpp"

using namespace std;
namespace ariel {

    /**
     * @brief Static map to store all intersections.
     */
    map<int, Intersection> Intersection::intersections;


    /**
     * @brief Initializes the intersections map if it is empty.
     */
    void Intersection::initialize() 
    {
        if (intersections.empty()) 
        {
            initializeIntersections();
        }
    }


     /**
     * @brief Checks if the intersection contains a specific vertex.
     * @param vertex The vertex to check.
     * @return bool True if the vertex is part of the intersection, false otherwise.
     */
    bool Intersection::contains(const Vertex& vertex) const 
    {
        return vertices.find(vertex) != vertices.end();
    }

    /**
     * @brief Getter for the intersection ID.
     * @return The unique identifier of the intersection.
     */
    int Intersection::getId() const 
    { 
        return id; 
    }


    /**
     * @brief Getter for the vertices of the intersection.
     * @return A const reference to the set of vertices defining the intersection.
     */
    const set<Vertex>& Intersection::getVertices() const 
    { 
        return vertices; 
    }


    /**
     * @brief Compares two intersections for ordering based on their vertices.
     * @param other The other intersection to compare with.
     * @return bool True if this intersection's vertices are lexicographically less than the other's.
     */
    bool Intersection::operator<(const Intersection& other) const 
    {
        return lexicographical_compare(vertices.begin(), vertices.end(), other.vertices.begin(), other.vertices.end());
    }


    /**
     * @brief Compares two intersections for equality based on their vertices.
     * @param other The other intersection to compare with.
     * @return bool True if the intersections have the same vertices, false otherwise.
     */
    bool Intersection::operator==(const Intersection& other) const 
    {
        return vertices == other.vertices;
    }

    /**
     * @brief Outputs the intersection details to an output stream.
     * @param os The output stream to write to.
     * @param intersection The intersection to print.
     * @return ostream& The output stream with the intersection details.
     */
    ostream& operator<<(ostream& os, const Intersection& intersection) 
    {
        os << "{ ";
        for (const auto& vertex : intersection.vertices) {
            os << vertex << " ";
        }
        os << "}";
        return os;
    }


    /**
     * @brief Returns all intersections. 
     * @return A reference to the map of all intersections.
     */
    const map<int, Intersection>& Intersection::getAllIntersections() 
    {
        if (intersections.empty()) 
        {
            initializeIntersections();
        }
        return intersections;
    }


    /**
     * @brief Returns a specific intersection by its ID.
     * @param intersectionID The ID of the intersection to retrieve.
     * @return Intersection The intersection with the specified ID.
     * @throws out_of_range if the intersection ID is not found.
     */
    Intersection Intersection::getIntersection(int intersectionID) 
    {
        auto it = intersections.find(intersectionID);
        if (it == intersections.end()) 
        {
            throw out_of_range("Invalid intersection ID: " + to_string(intersectionID));
        }
        return it->second;
    }


    /**
     * @brief Initializes the intersections map with predefined intersections.
     * This function populates the intersections map with predefined intersections,
     * each defined by a set of vertices and a unique ID.
     */
    void Intersection::initializeIntersections() 
    {
        intersections = {
            
            // Function for single vertex: Tile Vertex (a+-10, b+-10) 

            {1, Intersection({Vertex(10, 12)}, 1)},                                 // Single vertex at (0, 2), I
            {2, Intersection({Vertex(-10, -8), Vertex(1, 2)}, 2)},                 // Single vertex at (0, 2), II
            {3, Intersection({Vertex(0, 2), Vertex(1, 2)}, 3)},                     
            {4, Intersection({Vertex(11, 12)}, 4)},                                 // Single vertex at (1, 2)
            {5, Intersection({Vertex(1, 2), Vertex(2, 2)}, 5)},                    
            {6, Intersection({Vertex(12, 12)}, 6)},                                 // Single vertex at (2, 2), I
            {7, Intersection({Vertex(-8, -8)}, 7)},                                 // Single vertex at (2, 2), II
            {8, Intersection({Vertex(-11, -9)}, 8)},                                // Single vertex at (-1, 1)
            {9, Intersection({Vertex(-1, 1), Vertex(0, 2)}, 9)},                    
            {10, Intersection({Vertex(-1, 1), Vertex(0, 1), Vertex(0, 2)}, 10)},       
            {11, Intersection({Vertex(0, 2), Vertex(0, 1), Vertex(1, 2)}, 11)},
            {12, Intersection({Vertex(0, 1), Vertex(1, 2), Vertex(1, 1)}, 12)},
            {13, Intersection({Vertex(1, 2), Vertex(1, 1), Vertex(2, 2)}, 13)},
            {14, Intersection({Vertex(1, 1), Vertex(2, 2), Vertex(2, 1)}, 14)},
            {15, Intersection({Vertex(2, 2), Vertex(2, 1)}, 15)},
            {16, Intersection({Vertex(12, 11)}, 16)},                               // Single vertex at (2, 1)
            {17, Intersection({Vertex(8, 10)}, 17)},                                // Single vertex at (-2, 0), I
            {18, Intersection({Vertex(-2, 0), Vertex(-1, 1)}, 18)},
            {19, Intersection({Vertex(-2, 0), Vertex(-1, 1), Vertex(-1, 0)}, 19)},
            {20, Intersection({Vertex(-1, 1), Vertex(-1, 0), Vertex(0, 1)}, 20)},
            {21, Intersection({Vertex(-1, 0), Vertex(0, 1), Vertex(0, 0)}, 21)},
            {22, Intersection({Vertex(0, 1), Vertex(0, 0), Vertex(1, 1)}, 22)},
            {23, Intersection({Vertex(0, 0), Vertex(1, 1), Vertex(1, 0)}, 23)},
            {24, Intersection({Vertex(1, 1), Vertex(1, 0), Vertex(2, 1)}, 24)},
            {25, Intersection({Vertex(1, 0), Vertex(2, 1), Vertex(2, 0)}, 25)},
            {26, Intersection({Vertex(2, 0), Vertex(2, 1)}, 26)},
            {27, Intersection({Vertex(12, 10)}, 27)},                                // Single vertex at (2, 0), I
            {28, Intersection({Vertex(-12, -10)}, 28)},                              // Single vertex at (-2, 0), II
            {29, Intersection({Vertex(-2, -1), Vertex(-2, 0)}, 29)},
            {30, Intersection({Vertex(-2, 0), Vertex(-2, -1), Vertex(-1, 0)}, 30)},
            {31, Intersection({Vertex(-2, -1), Vertex(-1, 0), Vertex(-1, -1)}, 31)},
            {32, Intersection({Vertex(-1, 0), Vertex(-1, -1), Vertex(0, 0)}, 32)},
            {33, Intersection({Vertex(-1, -1), Vertex(0, 0), Vertex(0, -1)}, 33)},
            {34, Intersection({Vertex(0, 0), Vertex(0, -1), Vertex(1, 0)}, 34)},
            {35, Intersection({Vertex(-1, 0), Vertex(1, 0), Vertex(1, -1)}, 35)},
            {36, Intersection({Vertex(1, 0), Vertex(1, -1), Vertex(2, 0)}, 36)},
            {37, Intersection({Vertex(2, 0), Vertex(1, -1)}, 37)},
            {38, Intersection({Vertex(-8, -10)}, 38)},                               // Single vertex at (2, 0), II
            {39, Intersection({Vertex(8, 9)}, 39)},                                  // Single vertex at (-2, -1)
            {40, Intersection({Vertex(-2, -2), Vertex(-2, -1)}, 40)},
            {41, Intersection({Vertex(-2, -1), Vertex(-2, -2), Vertex(-1, -1)}, 41)},
            {42, Intersection({Vertex(-2, -2), Vertex(-1, -1), Vertex(-1, -2)}, 42)},
            {43, Intersection({Vertex(-1, -1), Vertex(-1, -2), Vertex(0, -1)}, 43)},
            {44, Intersection({Vertex(-1, -2), Vertex(0, -1), Vertex(0, -2)}, 44)},
            {45, Intersection({Vertex(0, -1), Vertex(0, -2), Vertex(1, -1)}, 45)},
            {46, Intersection({Vertex(1, -1), Vertex(0, -2)}, 46)},
            {47, Intersection({Vertex(11, 9)}, 47)},                                 // Single vertex at (1, -1)
            {48, Intersection({Vertex(10, 10)}, 48)},                                // Single vertex at (-2, -2), I
            {49, Intersection({Vertex(-12, -12)}, 49)},                              // Single vertex at (-2, -2), II
            {50, Intersection({Vertex(-1, -2), Vertex(-2, -2)}, 50)},
            {51, Intersection({Vertex(9, 8)}, 51)},                                  // Single vertex at (-1, -2)
            {52, Intersection({Vertex(0, -2), Vertex(-1, -2)}, 52)},
            {53, Intersection({Vertex(10, 8)}, 53)},                                 // Single vertex at (0, -2), I
            {54, Intersection({Vertex(-10, -12)}, 54)},                              // Single vertex at (0, -2), II
        };
    }
    
}