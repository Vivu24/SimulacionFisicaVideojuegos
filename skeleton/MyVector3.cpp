#include "MyVector3.h"


MyVector3 MyVector3::Normalize() const {
    float magnitude = Magnitude();
    if (magnitude == 0) {
        return MyVector3(0, 0, 0);
    }
    return MyVector3(x / magnitude, y / magnitude, z / magnitude);
}

float MyVector3::Magnitude() const {
    return std::sqrt(x * x + y * y + z * z);
}

float MyVector3::DotProduct(const MyVector3& other) const {
    return (x * other.x + y * other.y + z * other.z);
}

MyVector3 MyVector3::MultiplyByScalar(float scalar) const {
    return MyVector3(x * scalar, y * scalar, z * scalar);
}

MyVector3 MyVector3::operator=(const MyVector3& other) {
    this->x = other.x;
    this->y = other.y;
    this->z = other.z;

    return *this;
}

MyVector3 MyVector3::operator+(const MyVector3& other) const {
    return MyVector3(x + other.x, y + other.y, z + other.z);
}

MyVector3 MyVector3::operator-(const MyVector3& other) const {
    return MyVector3(x - other.x, y - other.y, z - other.z);
}

MyVector3 MyVector3::operator*(float scalar) const {
    return MyVector3(x * scalar, y * scalar, z * scalar);
}

bool MyVector3::operator==(const MyVector3& other) const {
    return (x == other.x && y == other.y && z == other.z);
}
