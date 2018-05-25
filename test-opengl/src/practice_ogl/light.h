#ifndef _LIGHT_H
#define _LIGHT_H
#include "math3d/math3d.h"

struct BaseLight {
	M3DVector3f m_color;
	float m_ambiance_intensity;
	float m_diffuse_intensity;

	BaseLight() {
		m3dLoadVector3(m_color, 0, 0, 0);
		m_ambiance_intensity = 0;
		m_diffuse_intensity = 0;
	}
};

struct DirectionLight: BaseLight {
	M3DVector3f m_direction;

	DirectionLight() {
		m3dLoadVector3(m_direction, 0, 0, 0);
	}
};


struct PointLight: BaseLight {
	struct Atten {
		float constant;
		float linear;
		float exp;
	};
	Atten atten;
	M3DVector3f position;

	PointLight() {
		atten.constant = 1;
		atten.linear = 0;
		atten.exp = 0;
	}
};
#endif // !_LIGHT_H
