#include "camera.h"
#include "glfw3\glfw3.h"
#include <stdio.h>

Camera::Camera(const int width, const int height): Camera(width, height, Vector3(0, 0, 0), Vector3(0, 0, 1), Vector3(0, 1, 0)) {
	;
}

Camera::Camera(const int width, const int height, const Vector3& pos, const Vector3& target, const Vector3& up) : 
	m_width(width), m_height(height), m_pos(pos), m_target(target), m_up(up) {
	//m3dCopyVector3(m_pos, pos);
	//m3dCopyVector3(m_target, target);
	//m3dCopyVector3(m_up, up);
	//m3dNormalizeVector3(m_target);
	//m3dNormalizeVector3(m_up);
	//m3dCrossProduct3(m_right, m_target, m_up);
	//m3dNormalizeVector3(m_right);

	m_target.normalize();
	m_up.normalize();
	m_right = m_target.cross_product(m_up);
	m_right.normalize();

	m_cam_rot_info = new CameraRotationInfo(width, height, 2000, m_target);
}

Camera::Camera(const Camera& cam): Camera(cam.m_width, cam.m_height, cam.m_pos, cam.m_target, cam.m_up){

}

void Camera::set_pos(float x, float y, float z) {
	m_pos[0] = x;
	m_pos[1] = y;
	m_pos[2] = z;
}

void Camera::set_target(const Vector3 & target)
{
	m_target = target;
	m_cam_rot_info->set_target(m_target);
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

void Camera::_move(Vector3& dir, float factor) {
	Vector3 step = dir;
	step = step * factor;
	m_pos = m_pos + step;

	//M3DVector3f step;
	//m3dCopyVector3(step, dir);
	//m3dScaleVector3(step, factor);
	//m3dAddVectors3(m_pos, m_pos, step);
}

void Camera::on_render_cb() {
	m_cam_rot_info->update();
	m_target = m_cam_rot_info->get_forward();
	
	m_right = m_target.cross_product(m_up);
	m_right.normalize();
	m_up = m_right.cross_product(m_target);
	m_up.normalize();

	//m3dCrossProduct3(m_right, m_target, m_up);
	//m3dNormalizeVector3(m_right);
	//m3dCrossProduct3(m_up, m_right, m_target);
	//m3dNormalizeVector3(m_up);
}

CameraRotationInfo::CameraRotationInfo(float width, float height, float factor, Vector3& target):
	m_win_width(width), m_win_height(height), m_left_edge(false), m_right_edge(false), 
	m_lower_edge(false), m_upper_edge(false), m_factor(factor){
	set_target(target);
}

void CameraRotationInfo::set_target(Vector3 target)
{
	// 次数假设target已经normalize
	float h_len = sqrtf(target[0] * target[0] + target[2] * target[2]);
	float h_angle = asin(target[2] / h_len);
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
	m_v_angle = asin(target[1] / target.get_length());

	// 光标
	m_mouse_x = -1;
	m_mouse_y = -1;
}

void CameraRotationInfo::on_mouse_move(double x, double y) {
	//printf("%f, %f\n", x, y);
	if (m_mouse_x < 0) {
		m_mouse_x = x;
		m_mouse_y = y;
		return;
	}

	float delta_x = x - m_mouse_x;
	float delta_y = y - m_mouse_y;

	m_mouse_x = x;
	m_mouse_y = y;

	m_h_angle -= delta_x / m_factor;
	m_v_angle -= delta_y / m_factor;

	if (delta_x == 0) {
		m_left_edge = x <= MARGIN;
		m_right_edge = x >= m_win_width - MARGIN;
	}
	else {
		m_left_edge = m_right_edge = false;
	}
	if (delta_y == 0) {
		m_lower_edge = y >= m_win_height - MARGIN;
		m_upper_edge = y <= MARGIN;
	}
	else {
		m_upper_edge = m_lower_edge = false;
	}
}

void CameraRotationInfo::update() {
	if (m_left_edge) m_h_angle -= EDGE_SPEED;
	if (m_right_edge) m_h_angle += EDGE_SPEED;
	if (m_upper_edge) m_v_angle += EDGE_SPEED;
	if (m_lower_edge) m_v_angle -= EDGE_SPEED;

	//printf("%d, %d, %d, %d\n", m_left_edge, m_right_edge, m_lower_edge, m_upper_edge);

	m_forward[0] = cos(m_h_angle);
	m_forward[2] = sin(m_h_angle);
	m_forward[1] = tan(m_v_angle);

	m_forward.normalize();
}