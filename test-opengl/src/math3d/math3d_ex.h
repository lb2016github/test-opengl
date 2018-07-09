#ifndef _MATH3D_EXTEND_H
#define _MATH3D_EXTEND_H
#include "math3d.h"

#define VECTOR3(x, y, z) M3DVector3f

class Vector2
{
public:
	Vector2();
	Vector2(float x, float y);
	Vector2(const Vector2& v);
	~Vector2();

	Vector2& operator=(const Vector2& vector);

	float x();
	float y();
public:
	M3DVector2f data;
};

class Vector3;
class Matrix;

class Vector3
{
public:
	Vector3();
	Vector3(const Vector3& vector);
	Vector3(float x, float y, float z);
	~Vector3();

	// ���õĺ���
	void normalize();
	float x();
	float y();
	float z();

	// ��ʽ����ת��
	operator float*();
	/******************* ���� **********************/
	Vector3 cross_product(Vector3& vector);
	/***************** ��������� **********************/
	Vector3& operator=(const Vector3& vector);

	friend Vector3& operator-(Vector3& v, float factor);
	friend Vector3& operator-(Vector3& v1, Vector3& v2);
	friend Vector3& operator-(Vector3& vector);
	friend Vector3& operator+(Vector3& v1, Vector3& v2);
	friend Vector3& operator+(Vector3& v, float factor);
	friend Vector3& operator+(float factor, Vector3& v);
	friend float operator*(Vector3& v1, Vector3& v2);	// ���
	friend Vector3& operator*(Vector3& v, float factor);
	friend Vector3& operator*(float factor, Vector3& v);
	friend Vector3& operator*(Matrix& mtx, Vector3& v);

public:
	M3DVector3f data;
};

class Matrix
{
public:
	Matrix();
	Matrix(const Matrix& mtx);
	Matrix(const M3DMatrix44f& mtx);
	~Matrix();

	// ��ʽ����ת��
	operator float*();
	/***************** ��������� **********************/
	Matrix& operator=(const Matrix& mtx);
	friend Matrix& operator*(Matrix& mtx1, Matrix& mtx2);
	friend Vector3& operator*(Matrix& mtx, Vector3& v);


	/***************** �������� **********************/
	static Matrix make_translation_matrix(Vector3& pos);
	static Matrix make_scale_matrix(Vector3& scale);
	static Matrix make_rotation_matrix(Vector3& rotation);
	static Matrix make_camera_rotation_matrix(Vector3& target, Vector3& up);
	static Matrix make_per_project_matrix(float fFov, float fAspect, float zMin, float zMax);
	static Matrix make_orth_project_matrix(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax);

public:
	M3DMatrix44f data;
};

/***************** ��������� **********************/
Vector3& operator-(Vector3& v, float factor);
Vector3& operator-(float factor, Vector3& v);
Vector3& operator-(Vector3& v1, Vector3& v2);
Vector3& operator-(Vector3& vector);
Vector3& operator+(Vector3& v, float factor);
Vector3& operator+(float factor, Vector3& v);
Vector3& operator+(Vector3& v1, Vector3& v2);
float operator*(Vector3& v1, Vector3& v2);	// ���
Vector3& operator*(Vector3& v, float factor);
Vector3& operator*(float factor, Vector3& v);

Vector3& operator*(Matrix& mtx, Vector3& v);

Matrix& operator*(Matrix& mtx1, Matrix& mtx2);







#endif // !_MATH3D_EXTEND_H

