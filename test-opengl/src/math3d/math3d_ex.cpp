#include "math3d_ex.h"

/******************************************************
Vector2
******************************************************/
Vector2::Vector2() : x(0), y(0)
{
}

Vector2::~Vector2()
{
}

Vector2::Vector2(const Vector2& vector)
{
	x = vector.x;
	y = vector.y;
}

Vector2::Vector2(float x, float y) : x(x), y(y)
{

}

Vector2& Vector2::operator=(const Vector2& vector)
{
	if (this == &vector) return *this;
	x = vector.x;
	y = vector.y;
	return *this;
}


/******************************************************
Vector3
******************************************************/
Vector3::Vector3(): x(0), y(0), z(0)
{
}

Vector3::~Vector3()
{
}

Vector3::Vector3(const Vector3& vector)
{
	x = vector.x;
	y = vector.y;
	z = vector.z;
}

Vector3::Vector3(float x, float y, float z): x(x), y(y), z(z)
{

}

Vector3& Vector3::operator=(const Vector3& vector)
{
	if (this == &vector) return *this;
	x = vector.x;
	y = vector.y;
	z = vector.z;
	return *this;
}