#include "skybox.h"

SkyboxTechnique::SkyboxTechnique() {
	m_vertex_shader_path = "shaders/skybox.vert";
	m_fragment_shader_path = "shaders/skybox.frag";
}
SkyboxTechnique::~SkyboxTechnique() {

}
bool SkyboxTechnique::init() {
	if (!Technique::init()) return false;
	if (!add_shader(GL_VERTEX_SHADER, m_vertex_shader_path.c_str())) return false;
	if (!add_shader(GL_FRAGMENT_SHADER, m_fragment_shader_path.c_str())) return false;
	if (!finalize()) return false;

	m_wvp_location = glGetUniformLocation(m_program_id, "wvp");
	m_sampler_location = glGetUniformLocation(m_program_id, "g_sampler_skybox");

	if (m_wvp_location == GL_INVALID_VALUE || m_sampler_location == GL_INVALID_VALUE) {
		return false;
	}

	return true;
}

void SkyboxTechnique::set_wvp_trans(const M3DMatrix44f wvp) {
	glUniformMatrix4fv(m_wvp_location, 1, false, wvp);
}
void SkyboxTechnique::set_texture_unit(unsigned int texture_unit) {
	glUniform1i(m_sampler_location, texture_unit);
}

/**************************************
SKYBOX
***********************************/
Skybox::Skybox(const Camera* cam, const PersProjInfo& p): m_skybox_technique(NULL), m_skybox_tex(NULL), m_mesh(NULL) {
	m_cam = cam;
	m_pers_proj_info = p;
}
Skybox::~Skybox() {
	if (m_skybox_technique) delete m_skybox_technique;
	if (m_skybox_tex) delete m_skybox_tex;
	if (m_mesh) delete m_mesh;
}
bool Skybox::init(
	const std::string& pox_x_filepath,
	const std::string& pox_y_filepath,
	const std::string& pox_z_filepath,
	const std::string& neg_x_filepath,
	const std::string& neg_y_filepath,
	const std::string& neg_z_filepath
) {
	m_skybox_tex = new CubemapTexture(
		pox_x_filepath,
		pox_y_filepath,
		pox_z_filepath,
		neg_x_filepath,
		neg_y_filepath,
		neg_z_filepath
	);
	m_skybox_tex->load();

	m_skybox_technique = new SkyboxTechnique();
	m_skybox_technique->init();

	m_mesh = new Mesh();
	m_mesh->load_mesh("res/sphere.obj");
	return true;
}
void Skybox::render() {
	GLint old_depth_mode, old_cull_mode;
	glGetIntegerv(GL_CULL_FACE_MODE, &old_cull_mode);
	glGetIntegerv(GL_DEPTH_FUNC, &old_depth_mode);

	glDepthFunc(GL_LEQUAL);
	glCullFace(GL_FRONT);

	m_skybox_technique->enable();
	m_skybox_tex->bind(GL_TEXTURE0);

	Pipline pipline;
	pipline.set_scale(20);
	pipline.set_world_pos(m_cam->m_pos);
	pipline.set_camera_info(m_cam->m_pos, m_cam->m_target, m_cam->m_up);
	pipline.set_pers_proj_info(m_pers_proj_info);

	M3DMatrix44f wvp;
	pipline.get_pers_wvp_trans(wvp);

	m_skybox_technique->set_wvp_trans(wvp);
	m_skybox_technique->set_texture_unit(0);

	m_mesh->render();

	glDepthFunc(old_depth_mode);
	glCullFace(old_cull_mode);
}