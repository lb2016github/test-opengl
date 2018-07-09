#include "tutorial_spot_light.h"
#include "common/const.h"
#include "glfw3/glfw3.h"

#define FIElD_WIDTH 10
#define FIELD_DEPTH 20 

bool TutorialSpotLight::init() {
	m_proj_info.fov = m3dDegToRad(60);
	m_proj_info.height = WINDOW_HEIGHT;
	m_proj_info.width = WINDOW_WIDTH;
	m_proj_info.z_near = 1;
	m_proj_info.z_far = 50;

	Vector3 pos(0, 1, -3), target(0, 0, 1), up(0, 1, 0);
	m_cam = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, pos, target, up);

	m_mesh = new Mesh();
	m_mesh->load_mesh("res/box.obj");

	m_box = new Mesh();
	m_box->load_mesh("res/box.obj");

	m_tech = new SpotLightTechnique();
	if (!m_tech->init()) {
		printf("Error: init technique\n");
	}
	m_tech->enable();
	m_tech->set_texture_unit(0);

	m3dLoadVector3(m_direction_light.color, 1, 1, 1);
	m_direction_light.ambiance_intensity = 0;
	m_direction_light.diffuse_intensity = 0.1f;
	m3dLoadVector3(m_direction_light.direction, 1, -1, 0);

	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE | GL_DEPTH_TEST);

	return true;
}

// ��Ⱦ����
void TutorialSpotLight::render_scene_callback(float width, float height, float time) {
	glClear(GL_COLOR_BUFFER_BIT);

	m_cam->on_render_cb();

	m_pipline.set_world_pos(0, 0, 1);
	m_pipline.set_scale(1);
	//m_pipline.set_rotation(0, time / 5, 0);
	m_pipline.set_camera_info(m_cam->m_pos, m_cam->m_target, m_cam->m_up);

	m_pipline.set_pers_proj_info(m_proj_info);
	Matrix wvp = m_pipline.get_pers_wvp_trans();
	Matrix w = m_pipline.get_world_trans();

	std::vector<PointLight> pl(2);
	pl[0].diffuse_intensity = 0.5;
	m3dLoadVector3(pl[0].color, 1, 0, 0);
	m3dLoadVector3(pl[0].position, 3.0f, 1.0f, FIELD_DEPTH * (cosf(time) + 1.0f) / 2.0f);
	pl[0].atten.linear = 0.1;
	pl[0].atten.exp = 1;

	pl[1].diffuse_intensity = 0.5;
	m3dLoadVector3(pl[1].color, 0, 0, 1);
	m3dLoadVector3(pl[1].position, 7.0f, 1.0f, FIElD_WIDTH * (sinf(time) + 1.0f) / 2.0f);
	pl[1].atten.linear = 0.1;
	pl[1].atten.exp = 1;

	std::vector<SpotLight> sl(2);
	sl[0].diffuse_intensity = 0.9;
	m3dLoadVector3(sl[0].color, 0, 1, 1);
	m3dCopyVector3(sl[0].position, m_cam->m_pos);
	m3dCopyVector3(sl[0].direction, m_cam->m_target);
	sl[0].atten.linear = 0.1f;
	sl[0].cutoff = 10;

	sl[1].diffuse_intensity = 0.9;
	m3dLoadVector3(sl[1].color, 1, 1, 1);
	m3dLoadVector3(sl[1].position, 5.0f, 3.0f, 10.0f);
	m3dLoadVector3(sl[1].direction, 0.0f, -1.0f, 0.0f);
	sl[1].atten.linear = 0.1f;
	sl[1].cutoff = 20;

	m_tech->set_direction_light(m_direction_light);
	m_tech->set_eye_position(m_cam->m_pos);
	m_tech->set_point_lights(pl);
	m_tech->set_specular_parameter(0, 0);
	m_tech->set_transformation(wvp, w);
	m_tech->set_spot_lights(sl);

	m_mesh->render(NULL);
	m_box->render(NULL);
}

// ���̻ص�
void TutorialSpotLight::key_callback(int key, int scancode, int action, int mods) {
	m_cam->on_keyboard(key);

	if (action != GLFW_PRESS) return;

	switch (key)
	{
	case GLFW_KEY_UP:
		m_direction_light.ambiance_intensity += 0.1;
		m_direction_light.diffuse_intensity -= 0.1;
		break;
	case GLFW_KEY_DOWN:
		m_direction_light.ambiance_intensity -= 0.1;
		m_direction_light.diffuse_intensity += 0.1;
		break;
	default:
		break;
	}
}

// ����ƶ��ص�
void TutorialSpotLight::cursor_position_callback(double x, double y) {
	m_cam->on_mouse_move(x, y);
}