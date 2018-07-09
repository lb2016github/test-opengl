#ifndef _CAMERA_H
#define _CAMERA_H
#include "math3d/math3d_ex.h"

#define MARGIN 30
#define EDGE_SPEED 0.01

class CameraRotationInfo {
public:
	CameraRotationInfo(float width, float height, float factor, Vector3& target);
	void on_mouse_move(double x, double y);
	void update();
	Vector3 get_forward() { return m_forward; }

private:
	float m_h_angle, m_v_angle;	 // 水平角，竖直角
	bool m_left_edge, m_right_edge, m_upper_edge, m_lower_edge;	// 鼠标是否到达边缘
	float m_mouse_x, m_mouse_y;	// 光标位置
	float m_win_width, m_win_height;	// 窗口宽高
	float m_factor;		// 滑屏速度
	Vector3 m_forward;	// 朝向
};

class Camera {
public:
	Camera(const int width, const int height);
	Camera(const int width, const int height, const Vector3& pos, const Vector3& target, const Vector3& up);
	Camera(const Camera& cam);

	void on_render_cb();

	void set_pos(float x, float y, float z);

	void on_keyboard(int key);
	void on_mouse_move(double x, double y);

	Vector3 m_pos;
	Vector3 m_target;
	Vector3 m_up;

protected:
	void _move(Vector3& dir, float factor);

	CameraRotationInfo* m_cam_rot_info;

private:
	int m_width;
	int m_height;
	Vector3 m_right;
};

#endif // !_CAMERA_H
