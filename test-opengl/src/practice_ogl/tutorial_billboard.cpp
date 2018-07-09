#include "tutorial_billboard.h"
#include "common/const.h"
#include "glfw3/glfw3.h"
#include "common/pipline.h"

#define FIElD_WIDTH 10
#define FIELD_DEPTH 20 


bool TutorialBillboard::init() {
	m_proj_info.fov = m3dDegToRad(60);
	m_proj_info.height = WINDOW_HEIGHT;
	m_proj_info.width = WINDOW_WIDTH;
	m_proj_info.z_near = 1;
	m_proj_info.z_far = 50;

	Vector3 pos(0, 3, -5), target(0, -0.2, 1), up(0, 1, 0);
	m_cam = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, pos, target, up);

	m_billboard_list = new BillboardList();
	m_billboard_list->init("res/monster_hellknight.png", 1, 1);

	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE | GL_DEPTH_TEST);

	m_skybox = new Skybox(m_cam, m_proj_info);
	m_skybox->init(
		"res/sp3right.jpg",
		"res/sp3top.jpg",
		"res/sp3front.jpg",
		"res/sp3left.jpg",
		"res/sp3bot.jpg",
		"res/sp3back.jpg"
	);

	return true;
}

// 渲染场景
void TutorialBillboard::render_scene_callback(float width, float height, float time) {
	m_cam->on_render_cb();

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	render_pass();
	m_skybox->render();
}

// 键盘回调
void TutorialBillboard::key_callback(int key, int scancode, int action, int mods) {
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
void TutorialBillboard::cursor_position_callback(double x, double y) {
	m_cam->on_mouse_move(x, y);
}

void TutorialBillboard::render_pass() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Pipline pipline;
	pipline.set_camera_info(m_cam->m_pos, m_cam->m_target, m_cam->m_up);
	pipline.set_pers_proj_info(m_proj_info);
	Matrix vp = pipline.get_vp_trans();

	int rows = 5, cols = 5, distance = 3;
	std::vector<Vector3> pos_list(rows * cols);
	for (int r = 0; r < rows; ++r) {
		for (int c = 0; c < cols; ++c) {
			int idx = r * cols + c;
			pos_list[idx] = Vector3(c * distance, 0, r * distance);
		}
	}
	m_billboard_list->render(pos_list, m_cam->m_pos, vp);
}