#ifndef _PIPLINE_H
#define _PIPLINE_H

#include "math3d\math3d.h"

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

	M3DVector3f pos;
	M3DVector3f target;
	M3DVector3f up;
};

class Pipline
{
public:
	Pipline();
	~Pipline();

	void set_world_pos(M3DVector3f world_pos) { m3dCopyVector3(m_world_pos, world_pos); }
	void set_world_pos(float x, float y, float z);
	void set_scale(M3DVector3f scale) { m3dCopyVector3(m_scale, scale); }
	void set_scale(float scale);
	void set_rotation(M3DVector3f rotation) { m3dCopyVector3(m_rotation, rotation); }
	void set_rotation(float x, float y, float z);

	void set_pers_proj_info(PersProjInfo& pp_info) { m_pp_info = pp_info; }
	void set_pers_proj_info(float fov, float width, float height, float z_near, float z_far);
	void set_orthor_proj_info(OrthorProjInfo& op_info) { m_op_info = op_info; }
	void set_orthor_proj_info(float right, float left, float back, float top, float z_near, float z_far);
	void set_camera_info(CameraInfo& cam_info) { m_cam_info = cam_info; }
	void set_camera_info(M3DVector3f pos, M3DVector3f target, M3DVector3f up);

	void get_world_trans(M3DMatrix44f w);
	void get_view_trans(M3DMatrix44f v);
	void get_pers_proj_trans(M3DMatrix44f p);
	void get_orthor_proj_trans(M3DMatrix44f p);
	void get_wv_trans(M3DMatrix44f wv);

	void get_pers_wvp_trans(M3DMatrix44f wvp);
	void get_orthor_wvp_trans(M3DMatrix44f wvp);

private:
	M3DVector3f m_world_pos;
	M3DVector3f m_scale;
	M3DVector3f m_rotation;

	PersProjInfo m_pp_info;
	OrthorProjInfo m_op_info;
	
	CameraInfo m_cam_info;
};

#endif // !_PIPLINE_H
