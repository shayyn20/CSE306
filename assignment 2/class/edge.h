#include "vector.h"
#include <vector>

class Edge {  
public:
    std::vector<Vector> vertices;
	double length;

	Edge(Vector v1, Vector v2) {
        vertices.push_back(v1);
        vertices.push_back(v2);
        length = norm(v1 - v2);
    };
	
};	

