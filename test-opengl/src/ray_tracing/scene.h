#ifndef _SECNE_H
#define _SECNE_H
#include "math3d/math3d_ex.h"
#include <vector>

// ������
class Scene{
public:
	std::vector<Sphere> spheres;	// todo �����Ƚϼ򵥣���ģ�ͼ򵥵Ĵ浽vector���棬���������Ż�
	double intersect(Ray& ray);		// ���ؽ������t
};

// ��
class Sphere {
public:
	Sphere(Vector3 center, float radius) : center(center), radius(radius) {}
	double intersect(Ray& ray);	// ���ؽ������t
public:
	Vector3 center;
	float radius;
};


// ����
class Ray {
public:
	bool is_alive();

public:
	Vector3 color;
	Vector3 start;
	Vector3 forward;
};

class RayHeper {
public:
	RayHeper();
	~RayHeper();
	// ��������
	void emit_ray(Ray& ray, Scene& scene);
private:
	std::vector<Ray> tmp_rays;
};

#endif
