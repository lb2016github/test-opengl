#ifndef _MATRIX_H
#define _MATRIX_H

#ifndef _MATRIX_EXPORT
#define _MATRIX_API __declspec(dllexport)
#else
#define _MATRIX_API __declspec(dllimport)
#endif // !_MY_MATH_EXPORT


_MATRIX_API class Matrix {
public:
	// 第四列为0，0，0，1
	float m11, m12, m13;
	float m21, m22, m23;
	float m31, m32, m33;
	float t1, t2, t3;

	Matrix():
		m11(1.0f), m12(.0f), m13(.0f),
		m21(.0f), m22(1.0f), m23(.0f),
		m31(.0f), m32(.0f), m33(1.0f)
	{}

	void get_data(float* data);
};



#endif // !_MATRIX_H
