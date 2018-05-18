#include "camera.h"
#include "glfw\include\glfw3.h"

Camera::Camera(int width, int height, M3DVector3f pos, M3DVector3f target, M3DVector3f up) : m_width(width), m_height(height) {
	m3dCopyVector3(m_pos, pos);
	m3dCopyVector3(m_target, target);
	m3dCopyVector3(m_up, up);
}

void Camera::set_pos(float x, float y, float z) {
	m_pos[0] = x;
	m_pos[1] = y;
	m_pos[2] = z;
}

void Camera::set_target(float x, float y, float z) {
	m_target[0] = x;
	m_target[1] = y;
	m_target[2] = z;
}