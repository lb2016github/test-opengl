#include "scene.h"
#include <math.h>


RayHeper::RayHeper()
{

}

RayHeper::~RayHeper()
{

}

void RayHeper::emit_ray(Ray& ray, Scene& scene)
{

}

double Sphere::intersect(Ray& ray)
{
	Vector3 A = center - ray.start;					// 射线起点到圆形的向量
	Vector3 B = A * ray.forward * ray.forward;		// A到射线上的投影
	float len_a = A.get_length();
	float len_b = B.get_length();
	float delta_sqr = radius * radius + len_b * len_b - len_a * len_a;
	return delta_sqr > 0 ? len_b - sqrt(delta_sqr): 0;
}

double Scene::intersect(Ray& ray)
{
	float min_t = 1e20;
	for (std::vector<Sphere>::iterator iter = spheres.begin(); iter != spheres.end(); ++iter)
	{
		double tmp_t = iter->intersect(ray);
		if (tmp_t > 0 && tmp_t < min_t) min_t = tmp_t;
	}
	return min_t;
}
