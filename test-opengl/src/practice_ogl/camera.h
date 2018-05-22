#ifndef _CAMERA_H
#define _CAMERA_H
#include "math3d\math3d.h"

#define MARGIN 3
#define EDGE_SPEED 0.01

class CameraRotationInfo {
public:
	CameraRotationInfo(float width, float height, float factor, M3DVector3f target);
	void on_mouse_move(double x, double y);
	void update();
	void get_forward(M3DVector3f forward) { m3dCopyVector3(forward, m_forward); };

private:
	float m_h_angle, m_v_angle;	 // 水平角，竖直角
	bool m_left_edge, m_right_edge, m_upper_edge, m_lower_edge;	// 鼠标是否到达边缘
	float m_mouse_x, m_mouse_y;	// 光标位置
	float m_win_width, m_win_height;	// 窗口宽高
	float m_factor;		// 滑屏速度
	M3DVector3f m_forward;	// 朝向
};

class Camera {
public:
	Camera(int width, int height);
	Camera(int width, int height, M3DVector3f pos, M3DVector3f target, M3DVector3f up);

	void on_render_cb();

	void set_pos(float x, float y, float z);

	void on_keyboard(int key);
	void on_mouse_move(double x, double y);

	M3DVector3f m_pos;
	M3DVector3f m_target;
	M3DVector3f m_up;

protected:
	void _move(M3DVector3f dir, float factor);

	CameraRotationInfo* m_cam_rot_info;

private:
	int m_width;
	int m_height;
	M3DVector3f m_right;
};

#endif // !_CAMERA_H
