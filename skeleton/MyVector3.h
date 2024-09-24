#pragma once
#include <cmath>

class MyVector3
{
public:
    // Constructores
    MyVector3() : x(0), y(0), z(0) {};
    MyVector3(float x, float y, float z) : x(x), y(y), z(z) {};

    // Métodos
    MyVector3 Normalize() const;
    float Magnitude() const;
    float DotProduct(const MyVector3& other) const;
    MyVector3 MultiplyByScalar(float scalar) const;

    // Operadores sobrecargados
    MyVector3 operator=(const MyVector3& other);
    MyVector3 operator+(const MyVector3& other) const;
    MyVector3 operator-(const MyVector3& other) const;
    MyVector3 operator*(float scalar) const;
    bool operator==(const MyVector3& other) const;

private:
    float x, y, z;
};

