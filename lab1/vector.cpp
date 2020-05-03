#include <math.h> 
#include "vector.hpp"

Vector::Vector(double x, double y, double z) {
    this->coords[0] = x;
    this->coords[1] = y;
    this->coords[2] = z;
}

const double& Vector::operator[](int i) const { 
    return this->coords[i];
}

double& Vector::operator[](int i) {
    return this->coords[i];
}

Vector& Vector::operator+=(Vector& b) {
    coords[0] += b[0];
    coords[1] += b[1];
    coords[2] += b[2];
    return *this;
}

Vector operator+(const Vector& a, const Vector& b) {
    return Vector(a[0] + b[0], a[1] + b[1], a[2] + b[2]);
}

Vector operator-(const Vector& a, const Vector& b) {
    return Vector(a[0] - b[0], a[1] - b[1], a[2] - b[2]);
}

Vector operator+(const Vector& a, const double b) {
    return Vector(a[0] + b, a[1] + b, a[2] + b);
}

Vector operator-(const Vector& a, const double b) {
    return Vector(a[0] - b, a[1] - b, a[2] - b);
}

Vector operator*(const Vector& a, const double b) {
    return Vector(a[0]*b, a[1]*b, a[2]*b);
}

Vector operator/(const Vector& a, const double b) {
    return Vector(a[0]/b, a[1]/b, a[2]/b);
}

double dot(const Vector& a, const Vector& b) {
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

double square(const Vector& a) {
    return dot(a, a);
}

Vector cross(const Vector& a, const Vector& b) {
    return Vector(a[1] * b[2] - a[2] * b[1], a[2] * b[0] - a[0] * b[2], a[0] * b[1] - a[1] * b[0]);
}

Vector normalize(const Vector& a) {
    return a * (1/norm(a));
}

double norm(const Vector& a) {
    return sqrt(pow(a[0],2) + pow(a[1],2) + pow(a[2],2));
}