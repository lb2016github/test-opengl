#include <math.h>
#include "tutorial_practice.h"
#include "glfw3\glfw3.h"


// 初始化
TutorialPractice::TutorialPractice():m_mesh(NULL), m_tech(NULL), m_camera(NULL) {
	m_pp_info.fov = m3dDegToRad(60);
	m_pp_info.height = WINDOW_HEIGHT;
	m_pp_info.width = WINDOW_WIDTH;
	m_pp_info.z_near = 1;
	m_pp_info.z_far = 100;

	m_op_info.bottom = -2;
	m_op_info.top = 1;
	m_op_info.left = -1;
	m_op_info.right = 1;
	m_op_info.z_near = 1;
	m_op_info.z_far = 100;
}

bool TutorialPractice::init() {
	m_tech = new DirectionLightTechnique();
	m_tech->init();
	m_tech->enable();
	m_tech->set_texture_unit(0);

	M3DVector3f pos, target, up;
	pos[0] = 0, pos[1] = 0, pos[2] = -3;
	target[0] = 0, target[1] = 0, target[2] = 1;
	up[0] = 0, up[1] = 1, up[2] = 0;
	m_camera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, pos, target, up);

	pipline = new Pipline();

	m_mesh = new SimpleMesh();
	m_mesh->load_mesh("");

	m3dLoadVector3(m_dir_light.color, 1, 1, 1);
	m_dir_light.ambiance_intensity = 0;
	m_dir_light.diffuse_intensity = 0.2;
	m3dLoadVector3(m_dir_light.direction, 0, 0, 1);
	m3dNormalizeVector3(m_dir_light.direction);

	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	return true;
}

// 更新
void TutorialPractice::render_scene_callback(float width, float height, float delta_time) {

	glClear(GL_COLOR_BUFFER_BIT);

	m_camera->on_render_cb();

	M3DMatrix44f wvp, w;
	pipline->set_world_pos(0, 0, 1);
	pipline->set_scale(1);
	pipline->set_rotation(0, delta_time / 5, 0);
	pipline->set_camera_info(m_camera->m_pos, m_camera->m_target, m_camera->m_up);

	pipline->set_pers_proj_info(m_pp_info);
	pipline->get_pers_wvp_trans(wvp);
	pipline->get_world_trans(w);

	//pipline->set_orthor_proj_info(m_op_info);
	//pipline->get_orthor_wvp_trans(wvp);

	m_tech->set_light(m_dir_light);
	m_tech->set_eye_pos(m_camera->m_pos);
	m_tech->set_material_specular_intensity(1.0);
	m_tech->set_transformation(wvp, w);
	m_tech->set_specular_power(32);

	m_mesh->render();
}

// 键盘回调
void TutorialPractice::key_callback(int key, int scancode, int action, int mods) {
	m_camera->on_keyboard(key);

	if (action != GLFW_PRESS) return;

	switch (key)
	{
	case GLFW_KEY_UP:
		m_dir_light.ambiance_intensity += 0.1;
		m_dir_light.diffuse_intensity -= 0.1;
		break;
	case GLFW_KEY_DOWN:
		m_dir_light.ambiance_intensity -= 0.1;
		m_dir_light.diffuse_intensity += 0.1;
		break;
	default:
		break;
	}
}

void TutorialPractice::cursor_position_callback(double x, double y) {
	m_camera->on_mouse_move(x, y);
}


// 销毁
TutorialPractice::~TutorialPractice() {
	if (m_mesh) delete m_mesh;
	if (m_tech) delete m_tech;
	if (m_camera) delete m_camera;
}