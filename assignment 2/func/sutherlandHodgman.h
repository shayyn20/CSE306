#include "tools.h"

Vector intersect(Edge& edge, Vector& u, Vector& v) {
	Vector A = edge.vertices[0];
	Vector B = edge.vertices[1];
	Vector N(v[1] - u[1], u[0] - v[0]);
	double t = dot(u - A, N) / dot(B - A, N);
	return A + t * (B - A);
}

bool inside(Vector& point, Edge& edge) {
	Vector p1 = edge.vertices[0];
	Vector p2 = edge.vertices[1];
	Vector u = (p1 + p2)/2;
	Vector N(p2[1] - p1[1], p1[0] - p2[0]);
	return dot(point - u, N) <= 0;
}

// Clips the subjectPolygon by a convex clipPolygon.
Polygon sutherlandHodgman(Polygon& subj, Polygon& clip) {
	std::vector<Edge> clipEdges = clip.edges();
	for(int i = 0; i < int(clipEdges.size()); i++) { // For each edge of the clip polygon
		// Clip the subjectPolygon by a half-space
		Polygon out = Polygon();
		for (int j = 0; j < int(subj.vertices.size()); j++) { // For each vertex of the subject polygon
			// Test the subject polygon edge with vertices (i-1, i)
			Vector cur = subj.vertices[j];
			Vector prev = subj.vertices[(j>0)?(j-1):(int(subj.vertices.size())-1)];
			// Compute inter. between the infinite line supported by clipEdge and edge (i-1, i)
			Edge cur_edge = Edge(prev, cur);
			Vector intersection = intersect(cur_edge, clipEdges[i].vertices[0], clipEdges[i].vertices[1]);
			
			if (inside(cur, clipEdges[i])) {
				if (!inside(prev, clipEdges[i])) {
					// The subject polygon edge crosses the clip edge, and we leave the clipping area
					out.vertices.push_back(intersection);
				}
				out.vertices.push_back(cur);
			}
			else if (inside(prev, clipEdges[i])) {
				// The subject polygon edge crosses the clip edge, and we enter the clipping area
				out.vertices.push_back(intersection);
			}
		}
		subj = out;
	}
	return subj;
}