#include "voronoi.h"

// modified for power diagram
Vector intersect_pow(Edge& edge, Vector& Pi, Vector& Pj, double& wi, double& wj) {
	Vector M = (Pi + Pj) / 2;
	M = M + (wi - wj) * (Pj - Pi) / (2 * norm(Pi - Pj) * norm(Pi - Pj));
    Vector A = edge.vertices[0];
	Vector B = edge.vertices[1];
	double t = dot(M - A, Pi - Pj) / dot(B - A, Pi - Pj);
	return A + t * (B - A);
}

bool inside_pow(Vector& point, Vector& Pi, Vector& Pj, double& wi, double& wj) {
    Vector M = (Pi + Pj) / 2;
	M = M + (wi - wj) * (Pj - Pi) / (2 * norm(Pi - Pj) * norm(Pi - Pj));
	return dot(point - M, Pj - Pi) < 0;
}

// Clips the subjectPolygon by a convex clipPolygon.
Polygon sutherlandHodgman_pow(Polygon& subj, std::vector<Vector>& points, std::vector<double>& weights, int i) {
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

                Vector intersection = intersect_pow(cur_edge, points[i], points[j], weights[i], weights[j]);
                
                if (inside_pow(cur, points[i], points[j], weights[i], weights[j])) {
                    
			    	if (!inside_pow(prev, points[i], points[j], weights[i], weights[j])) {
			    		// The subject polygon edge crosses the clip edge, and we leave the clipping area
			    		out.vertices.push_back(intersection);
			    	}
			    	out.vertices.push_back(cur);
			    }
			    else if (inside_pow(prev, points[i], points[j], weights[i], weights[j])) {
			    	// The subject polygon edge crosses the clip edge, and we enter the clipping area
			    	out.vertices.push_back(intersection);
			    }
            }
            subj = out;
        }
	}
	return subj;
}