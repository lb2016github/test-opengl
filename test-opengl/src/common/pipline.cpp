#include "pipline.h"

CameraInfo::CameraInfo(const CameraInfo& cam_info) {
	pos = cam_info.pos;
	target = cam_info.target;
	up = cam_info.up;
}

CameraInfo& CameraInfo::operator=(const CameraInfo& cam_info) {
	pos = cam_info.pos;
	target = cam_info.target;
	up = cam_info.up;
	return *this;
}

void CameraInfo::set_cam_info(const Vector3& p_pos, const Vector3& p_target, const Vector3& p_up) {
	pos = p_pos;
	target = p_target;
	up = p_up;
	target.normalize();
	up.normalize();
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

void Pipline::set_camera_info(const Vector3& pos, const Vector3& target, const Vector3& up) {
	m_cam_info.set_cam_info(pos, target, up);
}

Matrix Pipline::get_world_trans() {
	Matrix trans = Matrix::make_translation_matrix(m_world_pos);
	Matrix scale = Matrix::make_scale_matrix(m_scale);
	Matrix rotation = Matrix::make_rotation_matrix(m_rotation);
	Matrix world_trans = trans * (scale * rotation);
	return world_trans;

	//M3DMatrix44f tmp_pos, tmp_scale, tmp_rotation, tmp_pr;
	//m3dTranslationMatrix44(tmp_pos, m_world_pos[0], m_world_pos[1], m_world_pos[2]);
	//m3dScaleMatrix44(tmp_scale, m_scale[0], m_scale[1], m_scale[2]);
	//m3dExtendRotationMatrix44(tmp_rotation, m_rotation[0], m_rotation[1], m_rotation[2]);

	//m3dMatrixMultiply44(tmp_pr, tmp_scale, tmp_rotation);
	//m3dMatrixMultiply44(w, tmp_pos, tmp_pr);
}

Matrix Pipline::get_view_trans() {
	Matrix trans = Matrix::make_translation_matrix(-m_cam_info.pos);
	Matrix rot = Matrix::make_camera_rotation_matrix(m_cam_info.target, m_cam_info.up);
	return rot * trans;

	//M3DMatrix44f pos, rot, irot;
	//m3dTranslationMatrix44(pos, -m_cam_info.pos[0], -m_cam_info.pos[1], -m_cam_info.pos[2]);
	//m3dExtendCameraMatrix44(rot, m_cam_info.target, m_cam_info.up);
	//m3dMatrixMultiply44(v, rot, pos);
}

Matrix Pipline::get_pers_proj_trans() {
	float aspect = m_pp_info.width / m_pp_info.height;
	//m3dExtendCratePerspectiveMatrix(p, m_pp_info.fov, aspect, m_pp_info.z_near, m_pp_info.z_far);
	return Matrix::make_per_project_matrix(m_pp_info.fov, aspect, m_pp_info.z_near, m_pp_info.z_far);
}

Matrix Pipline::get_orthor_proj_trans() {
	//m3dExtendMakeOrthographicMatrix(p, m_op_info.left, m_op_info.right, m_op_info.bottom, m_op_info.top, m_op_info.z_near, m_op_info.z_far);
	return Matrix::make_orth_project_matrix(m_op_info.left, m_op_info.right, m_op_info.bottom, m_op_info.top, m_op_info.z_near, m_op_info.z_far);
}

Matrix Pipline::get_vp_trans() {
	//M3DMatrix44f v, p;
	//get_view_trans(v);
	//get_pers_proj_trans(p);
	//m3dMatrixMultiply44(wv, p, v);

	return get_pers_proj_trans() * get_view_trans();

}

Matrix Pipline::get_pers_wvp_trans() {
	/*M3DMatrix44f w, vp;
	get_world_trans(w);
	get_vp_trans(vp);
	m3dMatrixMultiply44(wvp, vp, w);*/

	return get_vp_trans() * get_world_trans();
}

Matrix Pipline::get_orthor_wvp_trans() {
	//M3DMatrix44f w, v, p, tmp;
	//get_world_trans(w);
	//get_view_trans(v);
	//get_orthor_proj_trans(p);
	//m3dMatrixMultiply44(tmp, p, v);
	//m3dMatrixMultiply44(wvp, tmp, w);

	return get_orthor_proj_trans() * (get_view_trans() * get_world_trans());
}