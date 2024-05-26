// #ifndef VERTEX_EDGE_HPP
// #define VERTEX_EDGE_HPP

// #include <utility>
// #include <set>
// #include <algorithm>
// #include <initializer_list>
// #include <iostream>
// #include <map>

// using namespace std;
// namespace ariel {

//     struct Vertex 
//     {
//         int x, y;

//         // Constructor 
//         Vertex(int x = 0, int y = 0) : x(x), y(y) {}    

//         // Basic operators
//         bool operator<(const Vertex& other) const 
//         {
//             return x < other.x || (x == other.x && y < other.y);
//         }

//         bool operator==(const Vertex& other) const 
//         {
//             return x == other.x && y == other.y;
//         }

//         // Overload operator<< for Vertex
//         friend ostream& operator<<(ostream& os, const Vertex& vertex) {
//             os << "(" << vertex.x << ", " << vertex.y << ")";
//             return os;
//         }
//     };

//     struct Intersection 
//     {
//         set<Vertex> vertices;       // A set of vertices that define this intersection
//         int id;
        
//         // Constructors
//         Intersection() {} 
//         Intersection(const std::initializer_list<Vertex>& verts) : vertices(verts) {}

//         // Method to check if a given vertex is part of this intersection
//         bool contains(const Vertex& vertex) const 
//         {
//             return vertices.find(vertex) != vertices.end();
//         }

//         // Less-than operator to store in ordered data structures
//         bool operator<(const Intersection& other) const 
//         {
//             if (vertices.size() != other.vertices.size())
//                 return vertices.size() < other.vertices.size();
//             return std::lexicographical_compare(vertices.begin(), vertices.end(),
//                                                 other.vertices.begin(), other.vertices.end());
//         }

//         // Check equality
//         bool operator==(const Intersection& other) const
//         {
//             return vertices == other.vertices;
//         }

//         // Overload operator<< for Intersection
//         friend ostream& operator<<(ostream& os, const Intersection& intersection) {
//             os << "{ ";
//             for (const auto& vertex : intersection.vertices) {
//                 os << vertex << " ";
//             }
//             os << "}";
//             return os;
//         }
//     };

//     struct Edge 
//     {
//         Intersection i1, i2;

//         // Constructor 
//         Edge(const Intersection& a, const Intersection& b) : i1(a < b ? a : b), i2(a < b ? b : a) {}


//         // Basic operator
//         bool operator<(const Edge& other) const 
//         {
//             return i1 < other.i1 || (i1 == other.i1 && i2 < other.i2);
//         }

//         bool operator==(const Edge& other) const 
//                 {
//                     return (i1 == other.i1 && i2 == other.i2) || (i1 == other.i2 && i2 == other.i1);
//                 }

//         // Overload operator<< for Edge
//         friend ostream& operator<<(ostream& os, const Edge& edge) {
//             os << edge.i1 << " to " << edge.i2;
//             return os;
//         }


//         bool involvesIntersection(int intersectionID, const map<int, Intersection>& intersections) const {
//             auto it1 = intersections.find(intersectionID);
//             if (it1 == intersections.end()) {
//                 throw std::out_of_range("Invalid intersection ID provided");
//             }
//             return (i1 == it1->second || i2 == it1->second);
//         }
//     };
// }

// #endif
