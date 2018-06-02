#include "camera.h"
#include "glfw3\glfw3.h"
#include <stdio.h>

Camera::Camera(const int width, const int height){
	M3DVector3f pos, target, up;
	m3dLoadVector3(pos, 0, 0, 0);
	m3dLoadVector3(target, 0, 0, 1);
	m3dLoadVector3(up, 0, 1, 0);
	Camera(width, height, pos, target, up);
}

Camera::Camera(const int width, const int height, const M3DVector3f pos, const M3DVector3f target, const M3DVector3f up) : m_width(width), m_height(height) {
	m3dCopyVector3(m_pos, pos);
	m3dCopyVector3(m_target, target);
	m3dCopyVector3(m_up, up);
	m3dNormalizeVector3(m_target);
	m3dNormalizeVector3(m_up);
	m3dCrossProduct3(m_right, m_target, m_up);
	m3dNormalizeVector3(m_right);
	m_cam_rot_info = new CameraRotationInfo(width, height, 2000, m_target);
}

Camera::Camera(const Camera& cam) {
	Camera(cam.m_width, cam.m_height, cam.m_pos, cam.m_target, cam.m_up);
}

void Camera::set_pos(float x, float y, float z) {
	m_pos[0] = x;
	m_pos[1] = y;
	m_pos[2] = z;
}

void Camera::on_keyboard(int key) {
	float factor = 1;
	switch (key)
	{
	case GLFW_KEY_W:
		_move(m_target, factor);
		break;
	case GLFW_KEY_S:
		_move(m_target, -1 * factor);
		break;
	case GLFW_KEY_A:
		_move(m_right, -1 * factor);
		break;
	case GLFW_KEY_D:
		_move(m_right, factor);
		break;
	default:
		break;
	}
}

void Camera::on_mouse_move(double x, double y) {
	m_cam_rot_info->on_mouse_move(x, y);
}

void Camera::_move(M3DVector3f dir, float factor) {
	M3DVector3f step;
	m3dCopyVector3(step, dir);
	m3dScaleVector3(step, factor);
	m3dAddVectors3(m_pos, m_pos, step);
}

void Camera::on_render_cb() {
	m_cam_rot_info->update();
	m_cam_rot_info->get_forward(m_target);
	
	m3dCrossProduct3(m_right, m_target, m_up);
	m3dNormalizeVector3(m_right);
	m3dCrossProduct3(m_up, m_right, m_target);
	m3dNormalizeVector3(m_up);
}

CameraRotationInfo::CameraRotationInfo(float width, float height, float factor, M3DVector3f target): 
	m_win_width(width), m_win_height(height), m_left_edge(false), m_right_edge(false), 
	m_lower_edge(false), m_upper_edge(false), m_factor(factor){
	// 次数假设target已经normalize
	float h_angle = asin(target[2]);
	if (target[0] >= 0 && target[2] >= 0) {	// 第一象限
		m_h_angle = h_angle;
	}
	else if (target[0] >= 0 && target[2] < 0) { // 第四象限
		m_h_angle = M3D_2PI + h_angle;
	}
	else if (target[0] < 0 && target[2] >= 0) { // 第二象限
		m_h_angle = M3D_PI - h_angle;
	}
	else { // 第三象限
		m_h_angle = M3D_PI - h_angle;
	}

	// 竖直角度
	m_v_angle = asin(target[1]);

	// 光标
	m_mouse_x = width / 2;
	m_mouse_y = height / 2;

}

void CameraRotationInfo::on_mouse_move(double x, double y) {
	//printf("%f, %f\n", x, y);

	float delta_x = x - m_mouse_x;
	float delta_y = y - m_mouse_y;

	m_mouse_x = x;
	m_mouse_y = y;

	m_h_angle += delta_x / m_factor;
	m_v_angle += delta_y / m_factor;

	if (delta_x == 0) {
		m_left_edge = x <= MARGIN;
		m_right_edge = x >= m_win_width - MARGIN;
	}
	if (delta_y == 0) {
		m_upper_edge = y >= m_win_height - MARGIN;
		m_lower_edge = y <= MARGIN;
	}
}

void CameraRotationInfo::update() {
	if (m_left_edge) m_h_angle += EDGE_SPEED;
	if (m_right_edge) m_h_angle -= EDGE_SPEED;
	if (m_upper_edge) m_v_angle += EDGE_SPEED;
	if (m_lower_edge) m_v_angle -= EDGE_SPEED;

	//printf("%d, %d, %d, %d\n", m_left_edge, m_right_edge, m_lower_edge, m_upper_edge);

	m_forward[0] = cos(m_h_angle);
	m_forward[2] = sin(m_h_angle);
	m_forward[1] = sin(m_v_angle);

	m3dNormalizeVector3(m_forward);
}