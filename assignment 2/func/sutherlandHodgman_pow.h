#include "voronoi.h"

// modified for power diagram
Vector intersect_pow(const Edge& edge, const Vector& Pi, const Vector& Pj, const double& wi, const double& wj) {
	Vector M = (Pi + Pj) / 2;
	M = M + (wi - wj) * (Pj - Pi) / (2 * norm(Pi - Pj) * norm(Pi - Pj));
    const Vector A = edge.vertices[0];
	const Vector B = edge.vertices[1];
	const double t = dot(M - A, Pi - Pj) / dot(B - A, Pi - Pj);
	return A + t * (B - A);
}

bool inside_pow(const Vector& point, const Vector& Pi, const Vector& Pj, const double& wi, const double& wj) {
    Vector M = (Pi + Pj) / 2;
	M = M + (wi - wj) * (Pj - Pi) / (2 * norm(Pi - Pj) * norm(Pi - Pj));
	return dot(point - M, Pj - Pi) < 0;
}

// Clips the subjectPolygon by a convex clipPolygon.
Polygon sutherlandHodgman_pow(Polygon& subj, const std::vector<Vector>& points, const std::vector<double>& weights, const int i) {
	for(int j = 0; j < int(points.size()); j++) { // For each Pj
		// Clip the subjectPolygon by a half-space
		Polygon out = Polygon();

        if (j != i) { // Pi != Pj
            for (int k = 0; k < int(subj.vertices.size()); k++) { // For each vertex of the subject polygon
                
			    // Test the subject polygon edge with vertices (i-1, i)
			    const Vector cur = subj.vertices[k];
			    const Vector prev = subj.vertices[(k>0)?(k-1):(int(subj.vertices.size())-1)];
			    // Compute inter. between the infinite line supported by clipEdge and edge (i-1, i)
			    const Edge cur_edge = Edge(prev, cur);

                const Vector intersection = intersect_pow(cur_edge, points[i], points[j], weights[i], weights[j]);
                
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