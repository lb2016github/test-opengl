#ifndef _LIGHT_H
#define _LIGHT_H
#include "math3d/math3d.h"

struct BaseLight {
	M3DVector3f m_color;
	float m_ambiance_intensity;
	float m_diffuse_intensity;
};

struct DirectionLight: BaseLight {
	M3DVector3f m_direction;
};


struct PointLight: BaseLight {
	struct Atten {
		float constant;
		float linear;
		float exp;
	};
	Atten atten;
	M3DVector3f position;
};
#endif // !_LIGHT_H
