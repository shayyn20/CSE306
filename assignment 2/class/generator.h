#include <vector>
#include <random>
#include <stdint.h>

#include "particle.h"

bool isInRectangle(Vector point) {
	return point[0] >= 0 && point[1] >= 0 && point[0] <= 1 && point[1] <= 1;
}

bool isInCircle(Vector point, double r, Vector center) {
	double fx = point[0] - center[0];
	double fy = point[1] - center[1];
	return (fx*fx + fy*fy) <= r*r;
}

bool isVast(Vector point, std::vector<Vector> list, double minDist) {
	for (int i = 0; i < int(list.size()); i ++) {
		if (norm(point - list[i]) < minDist) {
			return false;
		}
	}
	return true;
}

class Generator {
public:
	Generator() = default;
    explicit Generator(uint32_t seed): gen_(seed) {}

	double randomDouble() {
		return static_cast<double>(dis_(gen_));
	}

	int randomInt(int maxValue) {
		std::uniform_int_distribution<> disInt(0, maxValue);
		return disInt(gen_);
	}

    Vector popRandom(std::vector<Vector>& points) {
        const int i = randomInt(static_cast<int>(points.size())-1);
        const Vector p = points[i];
        points.erase(points.begin() + i);
        return p;
    }

private:
	std::mt19937 gen_ = std::mt19937(std::random_device()());
	std::uniform_real_distribution<double> dis_ = std::uniform_real_distribution<double>(0.0f, 1.0f);
};

std::vector<double> generateRandomWeights(int numPoints, Generator& generator, double maxWeight = 0.1) {
	std::vector<double> sampleWeights;
	// double minDist = sqrt(double(numPoints)) / double(numPoints);
	for (int i = 0; i < numPoints; i ++) {
		sampleWeights.push_back(maxWeight * generator.randomDouble());
	}
	return sampleWeights;
}


std::vector<Vector> generateRandomPoints(int numPoints, Generator& generator, bool isCircle = true, double r = 0.5, Vector center = Vector(0.5, 0.5)) {
	std::vector<Vector> samplePoints;
	for (int i = 0; i < numPoints; i ++) {
		Vector point;
		do {
			point = Vector(generator.randomDouble(), generator.randomDouble());
		} while (!(isCircle ? isInCircle(point, r, center) : isInRectangle(point)));
		samplePoints.push_back(point);
	}
	return samplePoints;
}

std::vector<Vector> generateVastRandomPoints(int numPoints, Generator& generator, bool isCircle = true, double r = 0.5, Vector center = Vector(0.5, 0.5)) {
	std::vector<Vector> samplePoints;
	double minDist = sqrt(double(numPoints)) / double(numPoints);
	for (int i = 0; i < numPoints; i ++) {
		Vector point;
		do {
			point = Vector(generator.randomDouble(), generator.randomDouble());
		} while (!(isCircle ? isInCircle(point, r, center) : isInRectangle(point)) && !isVast(point, samplePoints, minDist));
		samplePoints.push_back(point);
	}
	return samplePoints;
}