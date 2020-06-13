#include "edge.h"

class Polygon {  
public:
	std::vector<Vector> vertices;
	std::vector<Edge> edges() {
		std::vector<Edge> out;
		for (int i = 0; i < int(vertices.size()); i++) {
			Vector cur = vertices[i];
			Vector prev = vertices[(i>0)?(i-1):(vertices.size()-1)];
			out.push_back(Edge(prev, cur));
		}
		return out;
	};	
	Polygon() {};
	Polygon(std::vector<Vector> points) {
		for (int i = 0; i < int(points.size()); i++) {
			vertices.push_back(points[i]);
		}
	};
};