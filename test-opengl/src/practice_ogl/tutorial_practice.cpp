#include <math.h>
#include "tutorial_practice.h"
#include "glfw\include\glfw3.h"


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
	m_op_info.z_far = 1000;
}

bool TutorialPractice::init() {
	m_tech = new AmbianceLightTechnique();
	m_tech->init();
	m_tech->enable();

	M3DVector3f pos, target, up;
	pos[0] = 0, pos[1] = 0, pos[2] = 0;
	target[0] = 0, target[1] = 0, target[2] = 1;
	up[0] = 0, up[1] = 1, up[2] = 0;
	m_camera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, pos, target, up);

	pipline = new Pipline();

	m_mesh = new Mesh();
	m_mesh->load_mesh();

	return true;
}

// 更新
void TutorialPractice::render_scene_callback(float width, float height, float delta_time) {

	glClear(GL_COLOR_BUFFER_BIT);

	m_camera->set_pos(0, 0, -10);

	M3DMatrix44f wvp;
	pipline->set_world_pos(0, 0, 0);
	pipline->set_scale(1);
	pipline->set_rotation(0, 0, delta_time);
	pipline->set_camera_info(m_camera->m_pos, m_camera->m_target, m_camera->m_up);
	//pipline->get_wv_trans(wvp);

	//pipline->set_pers_proj_info(m_pp_info);
	//pipline->get_pers_wvp_trans(wvp);

	pipline->set_orthor_proj_info(m_op_info);
	pipline->get_orthor_wvp_trans(wvp);

	m_tech->set_ambiance_light();
	m_tech->set_wvp_trans(wvp);

	m_mesh->transform(wvp);
	m_mesh->render();
}

// 键盘回调
void TutorialPractice::key_callback(int key, int scancode, int action, int mods) {
	if (action != GLFW_PRESS) return;

	switch (key)
	{
	case GLFW_KEY_UP:
		m_tech->m_ambiance_intensity += 0.1;
		break;
	case GLFW_KEY_DOWN:
		m_tech->m_ambiance_intensity -= 0.1;
		break;
	default:
		break;
	}
}

// 销毁
TutorialPractice::~TutorialPractice() {
	if (m_mesh) delete m_mesh;
	if (m_tech) delete m_tech;
	if (m_camera) delete m_camera;
}

////////////////////////////////////AmbianceTechnique////////////////
bool AmbianceLightTechnique::init() {
	if (!Technique::init()) return false;
	// add shader
	if (!add_shader(GL_VERTEX_SHADER, "shaders/ambiance.vs")) return false;
	if (!add_shader(GL_FRAGMENT_SHADER, "shaders/ambiance.ps")) return false;
	// finalize
	if (!finalize()) return false;
	m_ambiance_color[0] = 1;
	m_ambiance_color[1] = 1;
	m_ambiance_color[2] = 1;
	m_ambiance_intensity = 1;

	wvp_location = glGetUniformLocation(m_program_id, "wvp");
	m_ambiance_color_location = glGetUniformLocation(m_program_id, "dir_light.color");
	m_ambiance_intensity_location = glGetUniformLocation(m_program_id, "dir_light.ambiant_intensity");

	return true;
}

void AmbianceLightTechnique::set_ambiance_light() {
	glUniform3f(m_ambiance_color_location, m_ambiance_color[0], m_ambiance_color[1], m_ambiance_color[2]);
	glUniform1f(m_ambiance_intensity_location, m_ambiance_intensity);
}

void AmbianceLightTechnique::set_wvp_trans(M3DMatrix44f wvp) {
	glUniformMatrix4fv(wvp_location, 1, false, wvp);
}