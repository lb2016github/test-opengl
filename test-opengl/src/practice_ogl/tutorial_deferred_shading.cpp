#include "tutorial_deferred_shading.h"
#include "common/utils.h"

TutorialDeferredShading::TutorialDeferredShading() {
	m_tech = NULL;
	m_dir_tech = NULL;
	m_cam = NULL;
	m_with_adjacencies = true;
}
TutorialDeferredShading::~TutorialDeferredShading() {
	SAFE_DELETE(m_tech);
	SAFE_DELETE(m_cam);
	SAFE_DELETE(m_dir_tech);
}

bool TutorialDeferredShading::init() {
	m_proj_info.fov = m3dDegToRad(60);
	m_proj_info.height = WINDOW_HEIGHT;
	m_proj_info.width = WINDOW_WIDTH;
	m_proj_info.z_near = 1;
	m_proj_info.z_far = 100;

	Vector3 pos(0, 0, 0), target(0, 0, 1), up(0, 1, 0);
	m_cam = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, pos, target, up);

	m_box.load_mesh("res/box.obj", m_with_adjacencies);
	m_quad.load_mesh("res/quad.obj");
	m_sphere.load_mesh("res/sphere.obj");

	m_gbuffer.init(WINDOW_WIDTH, WINDOW_HEIGHT);

	init_lights();
	init_tech();
	init_box_positions();

	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE | GL_DEPTH_TEST);

	return true;
}

void TutorialDeferredShading::init_lights() {
	m_dir_light.ambiance_intensity = 0.1f;
	m_dir_light.color = COLOR_CYAN;
	m_dir_light.diffuse_intensity = 0.5f;
	m_dir_light.direction = Vector3(1, 0, 0);

	m_point_lights[0].diffuse_intensity = 0.2f;
	m_point_lights[0].color= COLOR_GREEN;
	m_point_lights[0].position = Vector3(0.0f, 1.5f, 5.0f);
	m_point_lights[0].atten.constant = 0.0f;
	m_point_lights[0].atten.linear = 0.0f;
	m_point_lights[0].atten.exp = 0.3f;

	m_point_lights[1].diffuse_intensity = 0.2f;
	m_point_lights[1].color = COLOR_RED;
	m_point_lights[1].position = Vector3(2.0f, 0.0f, 5.0f);
	m_point_lights[1].atten.constant = 0.0f;
	m_point_lights[1].atten.linear = 0.0f;
	m_point_lights[1].atten.exp = 0.3f;

	m_point_lights[2].diffuse_intensity = 0.2f;
	m_point_lights[2].color = COLOR_BLUE;
	m_point_lights[2].position = Vector3(0.0f, 0.0f, 3.0f);
	m_point_lights[2].atten.constant = 0.0f;
	m_point_lights[2].atten.linear = 0.0f;
	m_point_lights[2].atten.exp = 0.3f;
}

void TutorialDeferredShading::init_tech() {
	m_tech = new DSGeometryTechnique();
	if (!m_tech->init()) {
		printf("Error: init tech\n");
	}
	m_dir_tech = new DSDirLightTechnique();
	if (!m_dir_tech->init()) {
		printf("Error: init DSDirLightTechnique\n");
	}
	m_point_tech = new DSPointLightTechnique();
	if (!m_point_tech->init()) {
		printf("Error: init DSPointLightTechnique\n");
	}
	m_dir_tech->enable();
	m_dir_tech->set_diffuse_sampler_index(GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);
	m_dir_tech->set_normal_sampler_index(GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);
	m_dir_tech->set_position_sampler_index(GBuffer::GBUFFER_TEXTURE_TYPE_POSITION);
	m_dir_tech->set_dir_light(m_dir_light);
	m_dir_tech->set_specular_param(0, 0);
	m_dir_tech->set_window_size(WINDOW_WIDTH, WINDOW_HEIGHT);

	m_point_tech->enable();
	m_point_tech->set_diffuse_sampler_index(GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);
	m_point_tech->set_normal_sampler_index(GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);
	m_point_tech->set_position_sampler_index(GBuffer::GBUFFER_TEXTURE_TYPE_POSITION);
	m_point_tech->set_specular_param(0, 0);
	m_point_tech->set_window_size(WINDOW_WIDTH, WINDOW_HEIGHT);
}

void TutorialDeferredShading::init_box_positions() {
	m_box_positions[0] = Vector3(0.0f, 0.0f, 5.0f);
	m_box_positions[1] = Vector3(6.0f, 1.0f, 10.0f);
	m_box_positions[2] = Vector3(-5.0f, -1.0f, 12.0f);
	m_box_positions[3] = Vector3(4.0f, 4.0f, 15.0f);
	m_box_positions[4] = Vector3(-4.0f, 2.0f, 20.0f);
}

// 渲染场景
void TutorialDeferredShading::render_scene_callback(float width, float height, float time) {
	ds_geom_pass(time);
	ds_begin_light_pass();
	ds_dir_light_pass();
	ds_point_light_pass();
}

void TutorialDeferredShading::ds_geom_pass(float time) {
	m_gbuffer.bind_for_writing();
	glDepthMask(GL_TRUE);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	m_tech->enable();
	Pipline pipline;
	pipline.set_rotation(0, time, 0);
	pipline.set_camera_info(m_cam->m_pos, m_cam->m_target, m_cam->m_up);
	pipline.set_pers_proj_info(m_proj_info);
	
	m_tech->set_tex_color_index(COLOR_TEXTURE_UNIT_INDEX);

	for (int i = 0; i < ELEMENTS_COUNT(m_box_positions); ++i) {
		pipline.set_world_pos(m_box_positions[i]);
		m_tech->set_world_trans(pipline.get_world_trans());
		m_tech->set_wvp_trans(pipline.get_pers_wvp_trans());
		GLenum mode = m_with_adjacencies ? GL_TRIANGLES_ADJACENCY : GL_TRIANGLES;
		m_box.render(NULL, mode);
	}

	glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);
}

void TutorialDeferredShading::ds_begin_light_pass() {
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	glBlendEquation(GL_FUNC_ADD);
	
	m_gbuffer.bind_for_reading();
	glClear(GL_COLOR_BUFFER_BIT);
}
void TutorialDeferredShading::ds_dir_light_pass() {
	m_dir_tech->enable();
	m_dir_tech->set_eye_pos(m_cam->m_pos);
	m_dir_tech->set_wvp_trans(Matrix());
	
	m_quad.render(NULL);
	
}

void TutorialDeferredShading::ds_point_light_pass() {
	m_point_tech->enable();
	m_point_tech->set_eye_pos(m_cam->m_pos);
	Pipline pipline;
	pipline.set_camera_info(m_cam->m_pos, m_cam->m_target, m_cam->m_up);
	pipline.set_pers_proj_info(m_proj_info);

	for (int i = 0; i < ELEMENTS_COUNT(m_point_lights); ++i) {
		float dis = calc_light_sphere_distance(m_point_lights[i]);
		pipline.set_scale(dis);
		pipline.set_world_pos(m_point_lights[i].position);
		m_point_tech->set_wvp_trans(pipline.get_pers_wvp_trans());
		m_point_tech->set_point_lights(1, m_point_lights, i);
		m_sphere.render(NULL);
	}
}

float TutorialDeferredShading::calc_light_sphere_distance(PointLight&light) {
	float linear = light.atten.linear;
	float exp = light.atten.exp;
	float constant = light.atten.constant;
	Vector3 color = light.color;
	float max_color = color[0] > color[1] ? color[0] : color[1];
	max_color = max_color > color[2] ? max_color : color[2];
	float intensity = light.diffuse_intensity;
	float distance = (-linear + sqrt(linear * linear - 4 * exp * (constant - 256 * max_color * intensity))) / (2 * exp);
	return distance;
}

void TutorialDeferredShading::ds_rend_pass() {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	m_gbuffer.bind_for_reading();

	GLsizei half_width = WINDOW_WIDTH / 2;
	GLsizei half_height = WINDOW_HEIGHT / 2;

	m_gbuffer.set_read_buffer(GBuffer::GBUFFER_TEXTURE_TYPE_POSITION);
	glBlitFramebuffer(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0, half_width, half_height, GL_COLOR_BUFFER_BIT, GL_LINEAR);

	m_gbuffer.set_read_buffer(GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);
	glBlitFramebuffer(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, half_width, 0, WINDOW_WIDTH, half_height, GL_COLOR_BUFFER_BIT, GL_LINEAR);

	m_gbuffer.set_read_buffer(GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);
	glBlitFramebuffer(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, half_height, half_width, WINDOW_HEIGHT, GL_COLOR_BUFFER_BIT, GL_LINEAR);

	m_gbuffer.set_read_buffer(GBuffer::GBUFFER_TEXTURE_TYPE_TEXCOORD);
	glBlitFramebuffer(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, half_width, half_height, WINDOW_WIDTH, WINDOW_HEIGHT, GL_COLOR_BUFFER_BIT, GL_LINEAR);
	
}

// 键盘回调
void TutorialDeferredShading::key_callback(int key, int scancode, int action, int mods) {
	m_cam->on_keyboard(key);
}

// 光标移动回调
void TutorialDeferredShading::cursor_position_callback(double x, double y) {
	m_cam->on_mouse_move(x, y);
}