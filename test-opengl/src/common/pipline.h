#ifndef _PIPLINE_H
#define _PIPLINE_H

#include "math3d/math3d_ex.h"

class PersProjInfo {
public:
	float fov;
	float width;
	float height;
	float z_near;
	float z_far;
};

class OrthorProjInfo {
public:
	float right;
	float left;
	float bottom;
	float top;
	float z_near;
	float z_far;
};

class CameraInfo {
public:
	CameraInfo() {}
	CameraInfo(const CameraInfo& cam_info);
	CameraInfo& operator=(const CameraInfo& cam_info);

	void set_cam_info(const Vector3& p_pos, const Vector3& p_target, const Vector3& p_up);


	Vector3 pos;
	Vector3 target;
	Vector3 up;
};

class Pipline
{
public:
	Pipline();
	~Pipline();

	void set_world_pos(const Vector3& world_pos) { m_world_pos = world_pos; }
	void set_world_pos(float x, float y, float z);
	void set_scale(const Vector3& scale) { m_scale = scale; }
	void set_scale(float scale);
	void set_rotation(const Vector3& rotation) { m_rotation = rotation; }
	void set_rotation(float x, float y, float z);

	void set_pers_proj_info(const PersProjInfo& pp_info) { m_pp_info = pp_info; }
	void set_pers_proj_info(float fov, float width, float height, float z_near, float z_far);
	void set_orthor_proj_info(const OrthorProjInfo& op_info) { m_op_info = op_info; }
	void set_orthor_proj_info(float right, float left, float back, float top, float z_near, float z_far);
	void set_camera_info(const CameraInfo& cam_info) { m_cam_info = cam_info; }
	void set_camera_info(const Vector3& pos, const Vector3& target, const Vector3& up);

	Matrix get_world_trans();
	Matrix get_view_trans();
	Matrix get_pers_proj_trans();
	Matrix get_orthor_proj_trans();
	Matrix get_vp_trans();
	Matrix get_pers_wvp_trans();
	Matrix get_orthor_wvp_trans();

private:
	Vector3 m_world_pos;
	Vector3 m_scale;
	Vector3 m_rotation;

	PersProjInfo m_pp_info;
	OrthorProjInfo m_op_info;
	
	CameraInfo m_cam_info;
};

#endif // !_PIPLINE_H
