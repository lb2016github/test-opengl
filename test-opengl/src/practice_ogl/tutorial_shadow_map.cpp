#include "tutorial_shadow_map.h"
#include "common/const.h"
#include "glfw3/glfw3.h"

#define FIElD_WIDTH 10
#define FIELD_DEPTH 20 


bool TutorialShadowMap::init() {
	m_proj_info.fov = m3dDegToRad(60);
	m_proj_info.height = WINDOW_HEIGHT;
	m_proj_info.width = WINDOW_WIDTH;
	m_proj_info.z_near = 1;
	m_proj_info.z_far = 500;

	m_spot_light.ambiance_intensity = 0;
	m_spot_light.diffuse_intensity = 0.9;
	m3dLoadVector3(m_spot_light.color, 1, 1, 1);
	m_spot_light.atten.linear = 0.01f;
	m3dLoadVector3(m_spot_light.position, -20, 20, 5);
	m3dLoadVector3(m_spot_light.direction, 1, -1, 0);
	m3dNormalizeVector3(m_spot_light.direction);
	m_spot_light.cutoff = 20;

	//m_spot_light.ambiance_intensity = 0;
	//m_spot_light.diffuse_intensity = 0.9;
	//m3dLoadVector3(m_spot_light.color, 1, 1, 1);
	//m_spot_light.atten.linear = 0.01f;
	//m3dLoadVector3(m_spot_light.position, -10, -10, 0);
	//m3dLoadVector3(m_spot_light.direction, 1, 1, 0);
	//m3dNormalizeVector3(m_spot_light.direction);
	//m_spot_light.cutoff = 20;

	M3DVector3f pos, target, up;
	pos[0] = 0, pos[1] = 10, pos[2] = -10;
	target[0] = 0, target[1] = -0.6, target[2] = 1;
	up[0] = 0, up[1] = 1, up[2] = 0;
	m_cam = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, pos, target, up);

	m_plane = new Mesh();
	m_plane->load_mesh("res/quad.obj");
	m_mesh = new Mesh();
	m_mesh->load_mesh("res/box.obj");

	m_shadow_map_tech = new ShadowMapTechnique();
	if (!m_shadow_map_tech->init()) {
		printf("Error: init shadow map technique\n");
	}
	m_shadow_map_tech->enable();

	m_shadow_map_fbo = new ShadowMapFBO();
	m_shadow_map_fbo->init(WINDOW_WIDTH, WINDOW_HEIGHT);

	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE | GL_DEPTH_TEST);

	return true;
}

// 渲染场景
void TutorialShadowMap::render_scene_callback(float width, float height, float time) {
	//m_cam->on_render_cb();

	shadow_map_pass();
	render_pass();

}

// 键盘回调
void TutorialShadowMap::key_callback(int key, int scancode, int action, int mods) {
	m_cam->on_keyboard(key);

	if (action != GLFW_PRESS) return;

	switch (key)
	{
	//case GLFW_KEY_UP:
	//	m_direction_light.ambiance_intensity += 0.1;
	//	m_direction_light.diffuse_intensity -= 0.1;
	//	break;
	//case GLFW_KEY_DOWN:
	//	m_direction_light.ambiance_intensity -= 0.1;
	//	m_direction_light.diffuse_intensity += 0.1;
	//	break;
	default:
		break;
	}
}

// 光标移动回调
void TutorialShadowMap::cursor_position_callback(double x, double y) {
	m_cam->on_mouse_move(x, y);
}

void TutorialShadowMap::shadow_map_pass() {
	m_shadow_map_fbo->bind_for_writing();
	glClear(GL_DEPTH_BUFFER_BIT);

	Pipline pipline;
	M3DVector3f up;
	m3dLoadVector3(up, 0, 0, 1);
	pipline.set_world_pos(0, 0, 5);
	pipline.set_rotation(m3dDegToRad(30), m3dDegToRad(30), 0);
	pipline.set_camera_info(m_spot_light.position, m_spot_light.direction, up);
	pipline.set_pers_proj_info(m_proj_info);
	M3DMatrix44f wvp;
	pipline.get_pers_wvp_trans(wvp);
	m_mesh->render();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void TutorialShadowMap::render_pass() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_shadow_map_tech->set_texture_unit(0);

	Pipline pipline;
	M3DMatrix44f wvp;
	pipline.set_world_pos(0, 0, 10);
	pipline.set_scale(10);
	pipline.set_rotation(m3dDegToRad(-90), 0, 0);
	pipline.set_camera_info(m_cam->m_pos, m_cam->m_target, m_cam->m_up);
	pipline.set_pers_proj_info(m_proj_info);
	pipline.get_pers_wvp_trans(wvp);
	m_plane->render();

}

////////////////////////////////////////////////////////

ShadowMapTechnique::ShadowMapTechnique() {
}

bool ShadowMapTechnique::init() {
	bool rst = SpotLightTechnique::init();

	m_light_wvp_location = glGetUniformLocation(m_program_id, "light_wvp");
	m_sampler_shadow_map_location = glGetUniformLocation(m_program_id, "g_sampler_shadow_map");

	if (m_light_wvp_location == INVALID_UNIFORM_LOCATION || m_light_wvp_location == INVALID_UNIFORM_LOCATION || !rst) return false;

	return true;
}

void ShadowMapTechnique::set_light_wvp_trans(M3DMatrix44f light_wvp) {
	glUniformMatrix4fv(m_light_wvp_location, 1, false, light_wvp);
}
void ShadowMapTechnique::init_shader_path() {
	m_vertex_shader_path = "shaders/shadow_map.vert";
	m_fragment_shader_path = "shaders/shadow_map.frag";
}