#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../class/generator.h"
#include "../lib/stb_image_write.h"
#include <string>
#include <iostream>
#include <sstream> 

// saves a static svg file. The polygon vertices are supposed to be in the range [0..1], and a canvas of size 1000x1000 is created
void save_svg(const std::vector<Polygon> &polygons, std::string filename, std::string fillcol = "none") {
	FILE* f = fopen(filename.c_str(), "w+"); 
	fprintf(f, "<svg xmlns = \"http://www.w3.org/2000/svg\" width = \"1000\" height = \"1000\">\n");
    for (int i=0; i<int(polygons.size()); i++) {
	    fprintf(f, "<g>\n");
	    fprintf(f, "<polygon points = \""); 
	    for (int j = 0; j < int(polygons[i].vertices.size()); j++) {
		    fprintf(f, "%3.3f, %3.3f ", (polygons[i].vertices[j][0] * 1000), (1000 - polygons[i].vertices[j][1] * 1000));
	    }
	    fprintf(f, "\"\nfill = \"%s\" stroke = \"black\"/>\n", fillcol.c_str());
	    fprintf(f, "</g>\n");
    }
	fprintf(f, "</svg>\n");
	fclose(f);
}

void save_svg_with_point(const std::vector<Polygon> &polygons, std::vector<Vector> &points, std::string filename, std::string fillcol = "none") {
	FILE* f = fopen(filename.c_str(), "w+"); 
	fprintf(f, "<svg xmlns = \"http://www.w3.org/2000/svg\" width = \"1000\" height = \"1000\">\n");
    for (int i=0; i<int(polygons.size()); i++) {
	    fprintf(f, "<g>\n");
	    fprintf(f, "<polygon points = \""); 
	    for (int j = 0; j < int(polygons[i].vertices.size()); j++) {
		    fprintf(f, "%3.3f, %3.3f ", (polygons[i].vertices[j][0] * 1000), (1000 - polygons[i].vertices[j][1] * 1000));
	    }
	    fprintf(f, "\"\nfill = \"%s\" stroke = \"black\"/>\n", fillcol.c_str());
	    fprintf(f, "</g>\n");
    }
	for (int i=0; i<int(points.size()); i++) {
	    fprintf(f, "<circle cx=\"%3.3f\" cy=\"%3.3f\" r=\"1\" stroke=\"red\" stroke-width=\"1\" fill=\"red\" />\n", (points[i][0] * 1000), (1000 - points[i][1] * 1000)); 
    }
	fprintf(f, "</svg>\n");
	fclose(f);
}


// Adds one frame of an animated svg file. frameid is the frame number (between 0 and nbframes-1).
// polygons is a list of polygons, describing the current frame.
// The polygon vertices are supposed to be in the range [0..1], and a canvas of size 1000x1000 is created
void save_svg_animated(const std::vector<Polygon> &polygons, std::string filename, int frameid, int nbframes, std::string fillcol = "none") {
	FILE* f;
	if (frameid == 0) {
		f = fopen(filename.c_str(), "w+");
		fprintf(f, "<svg xmlns = \"http://www.w3.org/2000/svg\" width = \"1000\" height = \"1000\">\n");
		fprintf(f, "<g>\n");
	} else {
		f = fopen(filename.c_str(), "a+");
	}
	fprintf(f, "<g>\n");
	for (int i = 0; i < int(polygons.size()); i++) {
		fprintf(f, "<polygon points = \""); 
		for (int j = 0; j < int(polygons[i].vertices.size()); j++) {
			fprintf(f, "%3.3f, %3.3f ", (polygons[i].vertices[j][0] * 1000), (1000-polygons[i].vertices[j][1] * 1000));
		}
		fprintf(f, "\"\nfill = \"%s\" stroke = \"black\"/>\n", fillcol.c_str());
	}
	fprintf(f, "<animate\n");
	fprintf(f, "	id = \"frame%u\"\n", frameid);
	fprintf(f, "	attributeName = \"display\"\n");
	fprintf(f, "	values = \"");
	for (int j = 0; j < nbframes; j++) {
		if (frameid == j) {
			fprintf(f, "inline");
		} else {
			fprintf(f, "none");
		}
		fprintf(f, ";");
	}
	fprintf(f, "none\"\n	keyTimes = \"");
	for (int j = 0; j < nbframes; j++) {
		fprintf(f, "%2.3f", j / (double)(nbframes));
		fprintf(f, ";");
	}
	fprintf(f, "1\"\n	dur = \"5s\"\n");
	fprintf(f, "	begin = \"0s\"\n");
	fprintf(f, "	repeatCount = \"indefinite\"/>\n");
	fprintf(f, "</g>\n");
	if (frameid == nbframes - 1) {
		fprintf(f, "</g>\n");
		fprintf(f, "</svg>\n");
	}
	fclose(f);
}

void save_svg_animated_with_point(const std::vector<Polygon> &polygons, std::vector<Vector> &points, std::string filename, int frameid, int nbframes, std::string fillcol = "none") {
	FILE* f;
	if (frameid == 0) {
		f = fopen(filename.c_str(), "w+");
		fprintf(f, "<svg xmlns = \"http://www.w3.org/2000/svg\" width = \"1000\" height = \"1000\">\n");
		fprintf(f, "<g>\n");
	} else {
		f = fopen(filename.c_str(), "a+");
	}
	fprintf(f, "<g>\n");
	for (int i = 0; i < int(polygons.size()); i++) {
		fprintf(f, "<polygon points = \""); 
		for (int j = 0; j < int(polygons[i].vertices.size()); j++) {
			fprintf(f, "%3.3f, %3.3f ", (polygons[i].vertices[j][0] * 1000), (1000-polygons[i].vertices[j][1] * 1000));
		}
		fprintf(f, "\"\nfill = \"%s\" stroke = \"black\"/>\n", fillcol.c_str());
	}
	for (int i=0; i<int(points.size()); i++) {
	    fprintf(f, "<circle cx=\"%3.3f\" cy=\"%3.3f\" r=\"1\" stroke=\"red\" stroke-width=\"1\" fill=\"red\" />\n", (points[i][0] * 1000), (1000 - points[i][1] * 1000)); 
    }
	fprintf(f, "<animate\n");
	fprintf(f, "	id = \"frame%u\"\n", frameid);
	fprintf(f, "	attributeName = \"display\"\n");
	fprintf(f, "	values = \"");
	for (int j = 0; j < nbframes; j++) {
		if (frameid == j) {
			fprintf(f, "inline");
		} else {
			fprintf(f, "none");
		}
		fprintf(f, ";");
	}
	fprintf(f, "none\"\n	keyTimes = \"");
	for (int j = 0; j < nbframes; j++) {
		fprintf(f, "%2.3f", j / (double)(nbframes));
		fprintf(f, ";");
	}
	fprintf(f, "1\"\n	dur = \"5s\"\n");
	fprintf(f, "	begin = \"0s\"\n");
	fprintf(f, "	repeatCount = \"indefinite\"/>\n");
	fprintf(f, "</g>\n");
	if (frameid == nbframes - 1) {
		fprintf(f, "</g>\n");
		fprintf(f, "</svg>\n");
	}
	fclose(f);
}

int sgn(double d){
    return d<0? -1 : d>0;
}

void save_frame(const std::vector<Polygon> &cells, std::string filename, int frameid = 0) {	
	int W = 1000, H = 1000;
	std::vector<unsigned char> image(W*H * 3, 255);
#pragma omp parallel for schedule(dynamic)
	for (int i = 0; i < int(cells.size()); i++) {
		double bminx = 1E9, bminy = 1E9, bmaxx = -1E9, bmaxy = -1E9;
		for (int j = 0; j < int(cells[i].vertices.size()); j++) {
			bminx = std::min(bminx, cells[i].vertices[j][0]);
			bminy = std::min(bminy, cells[i].vertices[j][1]);
			bmaxx = std::max(bmaxx, cells[i].vertices[j][0]);
			bmaxy = std::max(bmaxy, cells[i].vertices[j][1]);
		}
		bminx = std::min(W-1., std::max(0., W * bminx));
		bminy = std::min(H-1., std::max(0., H * bminy));
		bmaxx = std::max(W-1., std::max(0., W * bmaxx));
		bmaxy = std::max(H-1., std::max(0., H * bmaxy));
		for (int y = bminy; y < bmaxy; y++) {
			for (int x = bminx; x < bmaxx; x++) {
				int prevSign = 0;
				bool isInside = true;
				double mindistEdge = 1E9;
				for (int j = 0; j < int(cells[i].vertices.size()); j++) {
					double x0 = cells[i].vertices[j][0] * W;
					double y0 = cells[i].vertices[j][1] * H;
					double x1 = cells[i].vertices[(j + 1) % cells[i].vertices.size()][0] * W;
					double y1 = cells[i].vertices[(j + 1) % cells[i].vertices.size()][1] * H;
					double det = (x - x0)*(y1-y0) - (y - y0)*(x1-x0);
					int sign = sgn(det);
					if (prevSign == 0) prevSign = sign; else
						if (sign == 0) sign = prevSign; else
						if (sign != prevSign) {
							isInside = false;
							break;
						}
					prevSign = sign;
					double edgeLen = sqrt((x1 - x0)*(x1 - x0) + (y1 - y0)*(y1 - y0));
					double distEdge = std::abs(det)/ edgeLen;
					double dotp = (x - x0)*(x1 - x0) + (y - y0)*(y1 - y0);
					if (dotp<0 || dotp>edgeLen*edgeLen) distEdge = 1E9;
					mindistEdge = std::min(mindistEdge, distEdge);
				}
				if (isInside) {
					//if (i < N) {   // the N first particles may represent fluid, displayed in blue
					//	image[((H - y - 1)*W + x) * 3] = 0;
					//	image[((H - y - 1)*W + x) * 3 + 1] = 0;
					//	image[((H - y - 1)*W + x) * 3 + 2] = 255;
					//}
					if (mindistEdge <= 2) {
						image[((H - y - 1)*W + x) * 3] = 0;
						image[((H - y - 1)*W + x) * 3 + 1] = 0;
						image[((H - y - 1)*W + x) * 3 + 2] = 0;
					}
				}
				
			}
		}
	}
	std::ostringstream os;
	os << filename << frameid << ".png";
	stbi_write_png(os.str().c_str(), W, H, 3, &image[0], 0);
}