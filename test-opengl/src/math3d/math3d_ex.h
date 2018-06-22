#ifndef _MATH3D_EXTEND_H
#define _MATH3D_EXTEND_H
#include "math3d.h"

class Vector2
{
public:
	Vector2();
	Vector2(float x, float y);
	Vector2(const Vector2& v);
	~Vector2();

	Vector2& operator=(const Vector2& vector);

public:
	float x;
	float y;
};


class Vector3
{
public:
	Vector3();
	Vector3(const Vector3& vector);
	Vector3(float x, float y, float z);
	~Vector3();

	Vector3& operator=(const Vector3& vector);

public:
	float x;
	float y;
	float z;
};

#endif // !_MATH3D_EXTEND_H
