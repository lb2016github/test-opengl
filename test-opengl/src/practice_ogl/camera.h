#ifndef _CAMERA_H
#define _CAMERA_H
#include "math3d\math3d.h"


class Camera {
public:
	Camera(int width, int height) : m_width(width), m_height(height) {}
	Camera(int width, int height, M3DVector3f pos, M3DVector3f target, M3DVector3f up);

	void set_pos(float x, float y, float z);
	void set_target(float x, float y, float z);

	M3DVector3f m_pos;
	M3DVector3f m_target;
	M3DVector3f m_up;

private:
	int m_width;
	int m_height;
};

#endif // !_CAMERA_H
