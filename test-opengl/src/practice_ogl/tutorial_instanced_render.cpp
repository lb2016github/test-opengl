#include "tutorial_instanced_render.h"
#include "common/utils.h"
#include "math3d/math3d_ex.h"
#include <random>

#define NUM_ROWS 50
#define NUM_COLS 20
#define NUM_INSTANCES NUM_ROWS * NUM_COLS

TutorialInstancedRendering::TutorialInstancedRendering() {
	m_mesh = NULL;
	m_tech = NULL;
	m_cam = NULL;
	m_positions = NULL;
}
TutorialInstancedRendering::~TutorialInstancedRendering() {
	SAFE_DELETE(m_mesh);
	SAFE_DELETE(m_tech);
	SAFE_DELETE(m_cam);
	if (m_positions) {
		delete[] m_positions;
	}
}

bool TutorialInstancedRendering::init() {
	m_proj_info.fov = m3dDegToRad(60);
	m_proj_info.height = WINDOW_HEIGHT;
	m_proj_info.width = WINDOW_WIDTH;
	m_proj_info.z_near = 1;
	m_proj_info.z_far = 100;

	Vector3 pos(7, 3, 0), target(0, -0.2, 1), up(0, 1, 0);
	m_cam = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, pos, target, up);

	m_mesh = new VAOMesh();
	m_mesh->load_mesh("res/spider.obj");

	m_tech = new InstanceRenderingTechnique();
	if (!m_tech->init()) {
		printf("Error: init tech\n");
	}
	m_tech->enable();
	m_tech->set_tex_color_index(COLOR_TEXTURE_UNIT_INDEX);

	init_positions();

	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE | GL_DEPTH_TEST);

	return true;
}

void TutorialInstancedRendering::init_positions(){
	m_positions = new Vector3[NUM_INSTANCES];
	for (unsigned int i = 0; i < NUM_ROWS; i++) {
		for (unsigned int j = 0; j < NUM_COLS; j++) {
			unsigned int Index = i * NUM_COLS + j;
			m_positions[Index][0] = (float)j;
			m_positions[Index][1] = rand() / RAND_MAX * 5.0f;
			m_positions[Index][2] = (float)i;
		}
	}
}

// 渲染场景
void TutorialInstancedRendering::render_scene_callback(float width, float height, float time) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_tech->enable();

	Pipline p;
	p.set_camera_info(m_cam->m_pos, m_cam->m_target, m_cam->m_up);
	p.set_pers_proj_info(m_proj_info);
	p.set_rotation(0, m3dDegToRad(90), 0);
	p.set_scale(0.005);

	// 生成矩阵
	Matrix wvp_mtx_list[NUM_INSTANCES];
	Matrix world_mtx_list[NUM_INSTANCES];
	for (unsigned int i = 0; i < NUM_INSTANCES; i++) {
		M3DMatrix44f wvp, world;
		p.set_world_pos(m_positions[i][0], m_positions[i][1], m_positions[i][2]);
		world_mtx_list[i] = p.get_world_trans();
		wvp_mtx_list[i] = p.get_pers_wvp_trans();
	}
	m_mesh->render_instances(NULL, GL_TRIANGLES, NUM_INSTANCES, wvp_mtx_list, world_mtx_list);

}

// 键盘回调
void TutorialInstancedRendering::key_callback(int key, int scancode, int action, int mods) {
	m_cam->on_keyboard(key);
}

// 光标移动回调
void TutorialInstancedRendering::cursor_position_callback(double x, double y) {
	m_cam->on_mouse_move(x, y);
}