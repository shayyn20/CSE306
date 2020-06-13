#include "sh.h"

// modified for voronoi parallel linear enumeration
Vector intersect_vor(Edge& edge, Vector& Pi, Vector& Pj) {
	Vector M = (Pi + Pj) / 2;
    Vector A = edge.vertices[0];
	Vector B = edge.vertices[1];
	double t = dot(M - A, Pi - Pj) / dot(B - A, Pi - Pj);
	return A + t * (B - A);
}

bool inside_vor(Vector& point, Vector& Pi, Vector& Pj) {
    Vector M = (Pi + Pj) / 2;
	return dot(point - M, Pj - Pi) < 0;
}

// Clips the subjectPolygon by a convex clipPolygon.
Polygon sutherlandHodgman_vor(Polygon& subj, std::vector<Vector>& points, int i) {
	for(int j = 0; j < int(points.size()); j++) { // For each Pj
		// Clip the subjectPolygon by a half-space
		Polygon out = Polygon();

        if (j != i) { // Pi != Pj
            for (int k = 0; k < int(subj.vertices.size()); k++) { // For each vertex of the subject polygon
                
			    // Test the subject polygon edge with vertices (i-1, i)
			    Vector cur = subj.vertices[k];
			    Vector prev = subj.vertices[(k>0)?(k-1):(int(subj.vertices.size())-1)];
			    // Compute inter. between the infinite line supported by clipEdge and edge (i-1, i)
			    Edge cur_edge = Edge(prev, cur);

                Vector intersection = intersect_vor(cur_edge, points[i], points[j]);
                
                if (inside_vor(cur, points[i], points[j])) {
                    
			    	if (!inside_vor(prev, points[i], points[j])) {
			    		// The subject polygon edge crosses the clip edge, and we leave the clipping area
			    		out.vertices.push_back(intersection);
			    	}
			    	out.vertices.push_back(cur);
			    }
			    else if (inside_vor(prev, points[i], points[j])) {
			    	// The subject polygon edge crosses the clip edge, and we enter the clipping area
			    	out.vertices.push_back(intersection);
			    }
            }
            subj = out;
        }
	}
	return subj;
}