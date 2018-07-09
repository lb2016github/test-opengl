#include "tutorial_picking.h"
#include "common/const.h"
#include "glfw3/glfw3.h"
#include "common/pipline.h"
#include "common/utils.h"

#define FIElD_WIDTH 10
#define FIELD_DEPTH 20 

TutorialPicking::TutorialPicking() {
	m_cam = NULL;
	m_mesh = NULL;
	m_picking_technique = NULL;
	m_simple_color_technique = NULL;
	m_simple_show_technique = NULL;
	m_picking_texture = NULL;
	m_world_pos = std::vector<Vector3>(2);
	m_quad = NULL;
}
TutorialPicking::~TutorialPicking() {
	SAFE_DELETE(m_cam);
	SAFE_DELETE(m_mesh);
	SAFE_DELETE(m_picking_technique);
	SAFE_DELETE(m_simple_color_technique);
	SAFE_DELETE(m_simple_show_technique);
	SAFE_DELETE(m_picking_texture);
	SAFE_DELETE(m_quad);
}

bool TutorialPicking::init() {
	m_proj_info.fov = m3dDegToRad(60);
	m_proj_info.height = WINDOW_HEIGHT;
	m_proj_info.width = WINDOW_WIDTH;
	m_proj_info.z_near = 1;
	m_proj_info.z_far = 100;

	Vector3 pos(0, 5, -22), target(0, -0.2, 1), up(0, 1, 0);
	m_cam = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, pos, target, up);

	m_mesh = new Mesh();
	m_mesh->load_mesh("res/spider.obj");

	m_quad = new Mesh();
	m_quad->load_mesh("res/quad.obj");

	m_picking_technique = new PickingTechnique();
	if (!m_picking_technique->init()) {
		printf("Error: init PickingTechnique\n");
		return false;
	}

	m_simple_color_technique = new SimpleColorTechnique();
	if (!m_simple_color_technique->init()) {
		printf("Error: init SimpleColorTechnique\n");
		return false;
	}

	m_simple_show_technique = new SimpleShowTechnique();
	if (!m_simple_show_technique->init()) {
		printf("Error: init SimpleShowTechnique\n");
		return false;
	}
	m_simple_show_technique->set_tex_index(COLOR_TEXTURE_UNIT_INDEX);

	m_picking_texture = new PickingTexture();
	m_picking_texture->init(WINDOW_WIDTH, WINDOW_HEIGHT);

	m_left_mouse_button.is_pressed = false;

	m3dLoadVector3(m_world_pos[0], -10, 0, 5);
	m3dLoadVector3(m_world_pos[1], 10, 0, 5);

	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	return true;
}

// 键盘回调
void TutorialPicking::key_callback(int key, int scancode, int action, int mods) {
	m_cam->on_keyboard(key);

	if (action != GLFW_PRESS) return;

	switch (key)
	{
	default:
		break;
	}
}

// 光标移动回调
void TutorialPicking::cursor_position_callback(double x, double y) {
	m_cam->on_mouse_move(x, y);
	m_left_mouse_button.x = x;
	m_left_mouse_button.y = y;
}

// 渲染场景
void TutorialPicking::render_scene_callback(float width, float height, float time) {
	glClear(GL_COLOR_BUFFER_BIT);

	m_cam->on_render_cb();

	picking_pass();
	render_pass();
}

void TutorialPicking::mouse_button_callback(int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_1) {
		m_left_mouse_button.is_pressed = action == GLFW_PRESS;
	}
}

void TutorialPicking::picking_pass() {
	m_picking_texture->enable_writing();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	m_picking_technique->enable();
	Pipline pipline;
	pipline.set_scale(0.1);
	pipline.set_camera_info(m_cam->m_pos, m_cam->m_target, m_cam->m_up);
	pipline.set_pers_proj_info(m_proj_info);
	for (int i = 0; i < m_world_pos.size(); ++i) {
		pipline.set_world_pos(m_world_pos[i]);
		Matrix wvp = pipline.get_pers_wvp_trans();
		m_picking_technique->set_obj_id(i);
		m_picking_technique->set_wvp(wvp);
		
		m_mesh->render(m_picking_technique);
	}

	m_picking_texture->disable_writing();
}
void TutorialPicking::render_pass() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Pipline pipline;
	pipline.set_scale(0.1);
	pipline.set_camera_info(m_cam->m_pos, m_cam->m_target, m_cam->m_up);
	pipline.set_pers_proj_info(m_proj_info);

	// first render picking color if the mouse is pressed
	if (m_left_mouse_button.is_pressed) {
		PickingTexture::PixelInfo pixel_info = m_picking_texture->read_pixel(m_left_mouse_button.x, WINDOW_HEIGHT - m_left_mouse_button.y - 1);
		if (pixel_info.prim_id > 0) {
			pipline.set_world_pos(m_world_pos[pixel_info.obj_id]);
			Matrix wvp = pipline.get_pers_wvp_trans();

			m_simple_color_technique->enable();
			m_simple_color_technique->set_wvp(wvp);
			m_mesh->render_primitive(pixel_info.mesh_id, pixel_info.prim_id - 1);
		}
		//printf("Click: (obj_id, mesh_id, prim_id): %f, %f, %f\n", pixel_info.obj_id, pixel_info.mesh_id, pixel_info.prim_id);
	}

	// render obj
	for (int i = 0; i < m_world_pos.size(); ++i) {
		pipline.set_world_pos(m_world_pos[i]);
		Matrix wvp = pipline.get_pers_wvp_trans();

		m_simple_show_technique->enable();
		m_simple_show_technique->set_wvp(wvp);
		m_simple_show_technique->set_tex_index(COLOR_TEXTURE_UNIT_INDEX);

		m_mesh->render(NULL);
	}

	//// render quad
	//m_picking_texture->bind(COLOR_TEXTURE_UNIT);
	//pipline.set_world_pos(0, 0, -5);
	//pipline.set_scale(10);
	//M3DMatrix44f wvp;
	//pipline.get_pers_wvp_trans(wvp);

	//m_simple_show_technique->enable();
	//m_simple_show_technique->set_wvp(wvp);
	//m_simple_show_technique->set_tex_index(COLOR_TEXTURE_UNIT_INDEX);


	//m_quad->render(NULL);

}