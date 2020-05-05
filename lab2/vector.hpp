#pragma once
class Vector {
public: 
    explicit Vector(double x = 0., double y = 0., double z = 0.);
    Vector& operator+=(Vector& b);
    const double& operator[] (int i) const;
    double& operator[](int i);

private:
    double coords[3];
};

Vector operator+(const Vector& a, const Vector& b);
Vector operator-(const Vector& a, const Vector& b);
Vector operator+(const Vector& a, const double b);
Vector operator-(const Vector& a, const double b);
Vector operator*(const Vector& a, const double b);
Vector operator*(const Vector& a, const Vector& b);
Vector operator/(const Vector& a, const double b);
double dot(const Vector& a, const Vector& b);
double square(const Vector& a);
Vector cross(const Vector& a, const Vector& b);
Vector normalize(const Vector& a);
double norm(const Vector& a);