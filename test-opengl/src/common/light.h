#ifndef _LIGHT_H
#define _LIGHT_H
#include "math3d/math3d.h"

struct BaseLight {
	Vector3 color;
	float ambiance_intensity;
	float diffuse_intensity;

	BaseLight() {
		m3dLoadVector3(color, 0, 0, 0);
		ambiance_intensity = 0;
		diffuse_intensity = 0;
	}
};

struct DirectionLight: BaseLight {
	Vector3 direction;

	DirectionLight() {
		m3dLoadVector3(direction, 0, 0, 0);
	}
};


struct PointLight: BaseLight {
	struct Atten {
		float constant;
		float linear;
		float exp;
	};
	Atten atten;
	Vector3 position;

	PointLight() {
		atten.constant = 1;
		atten.linear = 0;
		atten.exp = 0;
	}
};

struct SpotLight : PointLight {
	Vector3 direction;
	float cutoff;	// 注：这里是角度

	SpotLight() {
		m3dLoadVector3(direction, 0, 0, 0);
		cutoff = 20;
	}
};

#endif // !_LIGHT_H
