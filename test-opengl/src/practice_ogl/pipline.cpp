#include "pipline.h"

CameraInfo::CameraInfo(const CameraInfo& cam_info) {
	m3dCopyVector3(pos, cam_info.pos);
	m3dCopyVector3(target, cam_info.target);
	m3dCopyVector3(up, cam_info.up);
}

CameraInfo& CameraInfo::operator=(const CameraInfo& cam_info) {
	m3dCopyVector3(pos, cam_info.pos);
	m3dCopyVector3(target, cam_info.target);
	m3dCopyVector3(up, cam_info.up);
	return *this;
}

Pipline::Pipline()
{
	m_world_pos[0] = 0;
	m_world_pos[1] = 0;
	m_world_pos[2] = 0;

	m_scale[0] = 1;
	m_scale[1] = 1;
	m_scale[2] = 1;

	m_rotation[0] = 0;
	m_rotation[1] = 0;
	m_rotation[2] = 0;
}

Pipline::~Pipline()
{
}

void Pipline::set_world_pos(float x, float y, float z) {
	m_world_pos[0] = x;
	m_world_pos[1] = y;
	m_world_pos[2] = z;
}

void Pipline::set_scale(float scale) {
	m_scale[0] = scale;
	m_scale[1] = scale;
	m_scale[2] = scale;
}

void Pipline::set_rotation(float x, float y, float z) {
	m_rotation[0] = x;
	m_rotation[1] = y;
	m_rotation[2] = z;
}

void Pipline::set_pers_proj_info(float fov, float width, float height, float z_near, float z_far) {
	m_pp_info.fov = fov;
	m_pp_info.width = width;
	m_pp_info.height = height;
	m_pp_info.z_near = z_near;
	m_pp_info.z_far = z_far;
}

void Pipline::set_orthor_proj_info(float right, float left, float bottom, float top, float z_near, float z_far) {
	m_op_info.right = right;
	m_op_info.left = left;
	m_op_info.bottom = bottom;
	m_op_info.top = top;
	m_op_info.z_near = z_near;
	m_op_info.z_far = z_far;
}

void Pipline::set_camera_info(M3DVector3f pos, M3DVector3f target, M3DVector3f up) {
	m3dCopyVector3(m_cam_info.pos, pos);
	m3dCopyVector3(m_cam_info.target, target);
	m3dCopyVector3(m_cam_info.up, up);
}

void Pipline::get_world_trans(M3DMatrix44f w) {
	M3DMatrix44f tmp_pos, tmp_scale, tmp_rotation, tmp_pr;
	m3dTranslationMatrix44(tmp_pos, m_world_pos[0], m_world_pos[1], m_world_pos[2]);
	m3dScaleMatrix44(tmp_scale, m_scale[0], m_scale[1], m_scale[2]);
	m3dRotationMatrix44(tmp_rotation, m_rotation[0], m_rotation[1], m_rotation[2]);

	m3dMatrixMultiply44(tmp_pr, tmp_scale, tmp_rotation);
	m3dMatrixMultiply44(w, tmp_pr, tmp_pos);
}

void Pipline::get_view_trans(M3DMatrix44f v) {
	M3DMatrix44f pos, rot;
	m3dTranslationMatrix44(pos, -m_cam_info.pos[0], -m_cam_info.pos[1], -m_cam_info.pos[2]);
	m3dCameraMatrix44(rot, m_cam_info.target, m_cam_info.up);
	m3dMatrixMultiply44(v, rot, pos);

}

void Pipline::get_wv_trans(M3DMatrix44f wv) {
	M3DMatrix44f w, v;
	get_world_trans(w);
	get_view_trans(v);
	m3dMatrixMultiply44(wv, v, w);
}

void Pipline::get_pers_proj_trans(M3DMatrix44f p) {
	float aspect = m_pp_info.width / m_pp_info.height;
	m3dMakePerspectiveMatrix(p, m_pp_info.fov, aspect, m_pp_info.z_near, m_pp_info.z_far);
}

void Pipline::get_orthor_proj_trans(M3DMatrix44f p) {
	m3dMakeOrthographicMatrix(p, m_op_info.left, m_op_info.right, m_op_info.bottom, m_op_info.top, m_op_info.z_near, m_op_info.z_far);
}

void Pipline::get_pers_wvp_trans(M3DMatrix44f wvp) {
	M3DMatrix44f w, v, p, tmp;
	get_world_trans(w);
	get_view_trans(v);
	get_pers_proj_trans(p);
	m3dMatrixMultiply44(tmp, p, v);
	m3dMatrixMultiply44(wvp, tmp, w);
}

void Pipline::get_orthor_wvp_trans(M3DMatrix44f wvp) {
	M3DMatrix44f w, v, p, tmp;
	get_world_trans(w);
	get_view_trans(v);
	get_orthor_proj_trans(p);
	m3dMatrixMultiply44(tmp, p, v);
	m3dMatrixMultiply44(wvp, tmp, w);
}