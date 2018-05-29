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
	m_proj_info.z_far = 50;

	m_spot_light.ambiance_intensity = 1;
	m_spot_light.diffuse_intensity = 0.9;
	m3dLoadVector3(m_spot_light.color, 1, 1, 1);
	m_spot_light.atten.linear = 0.01f;
	m3dLoadVector3(m_spot_light.position, -20, 20, 5);
	m3dLoadVector3(m_spot_light.direction, 1, -1, 0);
	m3dNormalizeVector3(m_spot_light.direction);
	m_spot_light.cutoff = 20;

	M3DVector3f pos, target, up;
	pos[0] = 3, pos[1] = 8, pos[2] = -10;
	target[0] = 0, target[1] = 1, target[2] = 1;
	up[0] = 0, up[1] = 1, up[2] = 0;
	m_cam = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, pos, target, up);

	m_plane = new Mesh();
	m_plane->load_mesh("res/quad.obj");
	m_mesh = new Mesh();
	m_mesh->load_mesh("res/phoenix_ugv.md2");


	m_tech = new SpotLightTechnique();
	if (!m_tech->init()) {
		printf("Error: init technique\n");
	}
	m_tech->enable();
	m_tech->set_texture_unit(0);

	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE | GL_DEPTH_TEST);

	return true;
}

// 渲染场景
void TutorialShadowMap::render_scene_callback(float width, float height, float time) {
	glClear(GL_COLOR_BUFFER_BIT);

	//m_cam->on_render_cb();

	M3DMatrix44f wvp, w;
	m_pipline.set_world_pos(0, 0, 3);
	m_pipline.set_scale(0.1);
	m_pipline.set_camera_info(m_cam->m_pos, m_cam->m_target, m_cam->m_up);
	m_pipline.set_pers_proj_info(m_proj_info);
	m_pipline.get_pers_wvp_trans(wvp);
	m_pipline.get_world_trans(w);

	std::vector<PointLight> pl;
	std::vector<SpotLight> sl;
	sl.push_back(m_spot_light);

	m_tech->set_eye_position(m_cam->m_pos);
	m_tech->set_point_lights(pl);
	m_tech->set_specular_parameter(0, 0);
	m_tech->set_transformation(wvp, w);
	m_tech->set_spot_lights(sl);

	m_plane->render();
	m_mesh->render();
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