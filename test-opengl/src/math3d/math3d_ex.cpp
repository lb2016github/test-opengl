#include "math3d_ex.h"

/******************************************************
Vector2
******************************************************/
Vector2::Vector2()
{
	m3dLoadVector2(data, 0, 0);
}

Vector2::~Vector2()
{
}

Vector2::Vector2(const Vector2& vector)
{
	m3dCopyVector2(data, vector.data);
}

Vector2::Vector2(float x, float y)
{
	m3dLoadVector2(data, x, y);
}

Vector2& Vector2::operator=(const Vector2& vector)
{
	if (this == &vector) return *this;
	m3dCopyVector2(data, vector.data);
	return *this;
}

float Vector2::x() {
	return data[0];
}
float Vector2::y() {
	return data[1];
}

/******************************************************
Vector3
******************************************************/
Vector3::Vector3(){
	m3dLoadVector3(data, 0, 0, 0);
}

Vector3::~Vector3()
{
}

Vector3::Vector3(const Vector3& vector)
{
	m3dCopyVector3(data, vector.data);
}

Vector3::Vector3(float x, float y, float z)
{
	m3dLoadVector3(data, x, y, z);
}

Vector3& Vector3::operator=(const Vector3& vector)
{
	if (this == &vector) return *this;
	m3dCopyVector3(data, vector.data);
	return *this;
}

// 常用的函数
void Vector3::normalize() {
	m3dNormalizeVector3(data);
}
float Vector3::x() {
	return data[0];
}
float Vector3::y() {
	return data[1];
}
float Vector3::z() {
	return data[2];
}

Vector3 Vector3::cross_product(Vector3& vector) {
	Vector3 rst;
	m3dCrossProduct3(rst, *this, vector);
	return rst;
}

Vector3::operator float*()
{
	return data;
}

/******************************************************
Matrix
******************************************************/
Matrix::Matrix()
{
	m3dLoadIdentity44(data);
}
Matrix::Matrix(const Matrix& mtx)
{
	m3dCopyMatrix44(data, mtx.data);
}
Matrix::Matrix(const M3DMatrix44f& mtx) {
	m3dCopyMatrix44(data, mtx);
}

Matrix::~Matrix()
{

}

Matrix& Matrix::operator=(const Matrix& mtx)
{
	if (this == &mtx) return *this;
	m3dCopyMatrix44(data, mtx.data);
	return *this;
}

Matrix::operator float*()
{
	return data;
}


/***************** 构建矩阵 **********************/
Matrix Matrix::make_translation_matrix(Vector3& pos)
{
	Matrix rst;
	m3dTranslationMatrix44(rst, pos[0], pos[1], pos[2]);
	return rst;
}
Matrix Matrix::make_scale_matrix(Vector3& scale) {
	Matrix rst;
	m3dScaleMatrix44(rst, scale[0], scale[1], scale[2]);
	return rst;
}
Matrix Matrix::make_rotation_matrix(Vector3& rotation) {
	Matrix rst;
	m3dExtendRotationMatrix44(rst, rotation[0], rotation[1], rotation[2]);
	return rst;
}
Matrix Matrix::make_camera_rotation_matrix(Vector3& target, Vector3& up) {
	Matrix rst;
	m3dExtendCameraMatrix44(rst, target, up);
	return rst;
}

Matrix Matrix::make_per_project_matrix(float fov, float aspect, float z_near, float z_far) {
	Matrix p;
	m3dExtendCratePerspectiveMatrix(p, fov, aspect, z_near, z_far);
	return p;
}
Matrix Matrix::make_orth_project_matrix(float left, float right, float bottom, float top, float z_near, float z_far){
	Matrix p;
	m3dExtendMakeOrthographicMatrix(p, left, right, bottom, top, z_near, z_far);
	return p;
}


/***************** 运算符重载 **********************/
Vector3& operator-(Vector3& v, float factor) {
	Vector3 rst;
	for (int i = 0; i < 3; ++i) {
		rst[i] = v[i] - factor;
	}
	return rst;
}
Vector3& operator-(float factor, Vector3& v) {
	Vector3 rst;
	for (int i = 0; i < 3; ++i) {
		rst[i] = v[i] - factor;
	}
	return rst;
}
Vector3& operator-(Vector3& v1, Vector3& v2) {
	Vector3 rst;
	for (int i = 0; i < 3; ++i) {
		rst[i] = v1[i] - v2[i];
	}
	return rst;
}
Vector3& operator-(Vector3& vector) {
	Vector3 rst;
	for (int i = 0; i < 3; ++i) {
		rst[i] = -vector[i];
	}
	return rst;
}
Vector3& operator+(Vector3& v1, Vector3& v2) {
	Vector3 rst;
	for (int i = 0; i < 3; ++i) {
		rst[i] = v1[i] + v2[i];
	}
	return rst;
}
Vector3& operator+(Vector3& v, float factor) {
	Vector3 rst;
	for (int i = 0; i < 3; ++i) {
		rst[i] = v[i] + factor;
	}
	return rst;
}
Vector3& operator+(float factor, Vector3& v) {
	Vector3 rst;
	for (int i = 0; i < 3; ++i) {
		rst[i] = v[i] + factor;
	}
	return rst;
}

// 点乘
float operator*(Vector3& v1, Vector3& v2) {
	return m3dDotProduct3(v1, v2);
}
Vector3& operator*(Vector3& v, float factor) {
	Vector3 rst;
	for (int i = 0; i < 3; ++i) {
		rst[i] = v[i] * factor;
	}
	return rst;
}
Vector3& operator*(float factor, Vector3& v) {
	Vector3 rst;
	for (int i = 0; i < 3; ++i) {
		rst[i] = v[i] * factor;
	}
	return rst;
}

Matrix& operator*(Matrix& mtx1, Matrix& mtx2) {
	Matrix rst;
	m3dMatrixMultiply44(rst, mtx1, mtx2);
	return rst;
}

Vector3& operator*(Matrix& mtx, Vector3& v) {
	Vector3 rst;
	m3dTransformVector3(rst, v, mtx);
	return rst;
}