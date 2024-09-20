#pragma once
class MyVector3
{
public:
	float x, y, z;

	MyVector3() {};
	MyVector3(float x, float y, float z) : x(x), y(y), z(z) {};
	

	float Normalize(float x, float y, float z);
	MyVector3 ScalarProduct();
	MyVector3 MultiplyByScalar();

	// Operadores =, +, -, *
	MyVector3 operator=(float x, float y, float z) const { return (x, y, z); }
	MyVector3 operator+(float x, float y, float z) const { return (x, y, z); }
	MyVector3 operator-(float x, float y, float z) const { return (x, y, z); }
	MyVector3 operator*(float x, float y, float z) const { return (x, y, z); }

private:

};

