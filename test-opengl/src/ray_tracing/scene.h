#ifndef _SECNE_H
#define _SECNE_H
#include "math3d/math3d_ex.h"
#include <vector>

// 场景类
class Scene{
public:
	std::vector<Sphere> spheres;	// todo 场景比较简单，把模型简单的存到vector里面，后续可以优化
	double intersect(Ray& ray);		// 返回交点参数t
};

// 球
class Sphere {
public:
	Sphere(Vector3 center, float radius) : center(center), radius(radius) {}
	double intersect(Ray& ray);	// 返回交点参数t
public:
	Vector3 center;
	float radius;
};


// 射线
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
	// 发射射线
	void emit_ray(Ray& ray, Scene& scene);
private:
	std::vector<Ray> tmp_rays;
};

#endif
