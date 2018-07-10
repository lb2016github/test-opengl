#include "tutorial_deferred_shading.h"
#include "common/utils.h"

TutorialDeferredShading::TutorialDeferredShading() {
	m_mesh = NULL;
	m_tech = NULL;
	m_cam = NULL;
}
TutorialDeferredShading::~TutorialDeferredShading() {
	SAFE_DELETE(m_mesh);
	SAFE_DELETE(m_tech);
	SAFE_DELETE(m_cam);
}

bool TutorialDeferredShading::init() {
	m_proj_info.fov = m3dDegToRad(60);
	m_proj_info.height = WINDOW_HEIGHT;
	m_proj_info.width = WINDOW_WIDTH;
	m_proj_info.z_near = 1;
	m_proj_info.z_far = 100;

	Vector3 pos(0, 0, 10), target(0, 0, -1), up(0, 1, 0);
	m_cam = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, pos, target, up);

	m_mesh = new VAOMesh();
	m_mesh->load_mesh("res/spider.obj");

	m_tech = new DSGeometryTechnique();
	if (!m_tech->init()) {
		printf("Error: init tech\n");
	}
	m_tech->enable();
	m_tech->set_tex_color_index(COLOR_TEXTURE_UNIT_INDEX);

	m_gbuffer.init(WINDOW_WIDTH, WINDOW_HEIGHT);

	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE | GL_DEPTH_TEST);

	return true;
}

// 渲染场景
void TutorialDeferredShading::render_scene_callback(float width, float height, float time) {
	ds_geom_pass(time);
	ds_rend_pass();
}

void TutorialDeferredShading::ds_geom_pass(float time) {
	m_gbuffer.bind_for_writing();
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	m_tech->enable();
	Pipline pipline;
	pipline.set_world_pos(0, 0, 0);
	pipline.set_scale(0.05);
	pipline.set_rotation(0, time, 0);
	pipline.set_camera_info(m_cam->m_pos, m_cam->m_target, m_cam->m_up);
	pipline.set_pers_proj_info(m_proj_info);
	Matrix world = pipline.get_world_trans();
	Matrix wvp = pipline.get_pers_wvp_trans();
	
	m_tech->set_tex_color_index(COLOR_TEXTURE_UNIT_INDEX);
	m_tech->set_world_trans(world);
	m_tech->set_wvp_trans(wvp);

	m_mesh->render(NULL);

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