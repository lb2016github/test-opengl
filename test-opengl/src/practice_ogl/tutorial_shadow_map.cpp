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

	m_spot_light.ambiance_intensity = 0.1;
	m_spot_light.diffuse_intensity = 0.9;
	m3dLoadVector3(m_spot_light.color, 1, 1, 1);
	m_spot_light.atten.linear = 0.01f;
	m3dLoadVector3(m_spot_light.position, -20, 20, 1);
	m3dLoadVector3(m_spot_light.direction, 1, -1, 0);
	m3dNormalizeVector3(m_spot_light.direction);
	m_spot_light.cutoff = 20;

	Vector3 pos(3, 8, -10), target(0, -0.2, 1), up(0, 1, 0);
	m_cam = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, pos, target, up);

	m_plane = new Mesh();
	m_plane->load_mesh("res/quad.obj");
	m_mesh = new Mesh();
	m_mesh->load_mesh("res/phoenix_ugv.md2");

	m_test_tex = new Texture(GL_TEXTURE_2D, "res/test.png");
	m_test_tex->load();

	m3dLoadVector3(m_mesh_pos, 0, 0, 3);
	m3dLoadVector3(m_mesh_rot, m3dDegToRad(30), m3dDegToRad(30), 0);
	m3dLoadVector3(m_mesh_scale, 0.1, 0.1, 0.1);

	m_shadow_map_tech = new ShadowMapTechnique();
	if (!m_shadow_map_tech->init()) {
		printf("Error: init shadow map technique\n");
	}

	m_shadow_map_fbo = new ShadowMapFBO();
	m_shadow_map_fbo->init(WINDOW_WIDTH, WINDOW_HEIGHT);

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

// ��Ⱦ����
void TutorialShadowMap::render_scene_callback(float width, float height, float time) {
	m_cam->on_render_cb();
	m3dLoadVector3(m_mesh_rot, 0, time / 10, 0);
	//m3dLoadVector3(m_mesh_pos, 0, 3 * sinf(time), 0);

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	shadow_map_pass();
	render_pass();
	m_skybox->render();
}

// ���̻ص�
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

// ����ƶ��ص�
void TutorialShadowMap::cursor_position_callback(double x, double y) {
	m_cam->on_mouse_move(x, y);
}

void TutorialShadowMap::shadow_map_pass() {
	m_shadow_map_tech->enable();
	m_shadow_map_fbo->bind_for_writing();
	glClear(GL_DEPTH_BUFFER_BIT);

	Pipline pipline;
	pipline.set_world_pos(m_mesh_pos);
	pipline.set_rotation(m_mesh_rot);
	pipline.set_scale(m_mesh_scale);
	pipline.set_camera_info(m_spot_light.position, m_spot_light.direction, Vector3(0, 1, 0));
	pipline.set_pers_proj_info(m_proj_info);
	Matrix wvp = pipline.get_pers_wvp_trans();
	Matrix w = pipline.get_world_trans();

	m_shadow_map_tech->set_transformation(wvp, w);
	m_mesh->render(NULL);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void TutorialShadowMap::render_pass() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	m_shadow_map_tech->enable();

	// spot light
	std::vector<SpotLight> spot_lights;
	spot_lights.push_back(m_spot_light);
	// dir light
	DirectionLight dir_light;
	m3dLoadVector3(dir_light.color, 0, 0, 0);
	// point lights
	std::vector<PointLight> point_lights;

	Pipline pipline;
	pipline.set_world_pos(0, 0, 1);
	pipline.set_scale(10);
	pipline.set_rotation(m3dDegToRad(90), 0, 0);
	Matrix w = pipline.get_world_trans();
	pipline.set_pers_proj_info(m_proj_info);
	pipline.set_camera_info(m_spot_light.position, m_spot_light.direction, Vector3(0, 1, 0));
	Matrix light_wvp = pipline.get_pers_wvp_trans();
	pipline.set_camera_info(m_cam->m_pos, m_cam->m_target, m_cam->m_up);
	Matrix wvp = pipline.get_pers_wvp_trans();

	// init technique
	m_shadow_map_tech->set_texture_unit(0);
	m_shadow_map_tech->set_shadow_map_tex_unit(1);
	m_shadow_map_tech->set_spot_lights(spot_lights);
	m_shadow_map_tech->set_point_lights(point_lights);
	m_shadow_map_tech->set_direction_light(dir_light);
	m_shadow_map_tech->set_transformation(wvp, w);
	m_shadow_map_tech->set_light_wvp_trans(light_wvp);
	m_shadow_map_tech->set_eye_position(m_cam->m_pos);
	m_shadow_map_tech->set_specular_parameter(0.5, 5);
	

	m_test_tex->bind(GL_TEXTURE0);
	m_shadow_map_fbo->bind_for_reading(GL_TEXTURE1);
	m_plane->render(NULL);
	
	// render mesh
	Pipline pipline_mesh;
	pipline_mesh.set_world_pos(m_mesh_pos);
	pipline_mesh.set_rotation(m_mesh_rot);
	pipline_mesh.set_scale(m_mesh_scale);
	Matrix w_mesh = pipline_mesh.get_world_trans();
	pipline_mesh.set_pers_proj_info(m_proj_info);
	pipline_mesh.set_camera_info(m_spot_light.position, m_spot_light.direction, Vector3(0, 1, 0));
	Matrix light_wvp_mesh = pipline_mesh.get_pers_wvp_trans();
	pipline_mesh.set_camera_info(m_cam->m_pos, m_cam->m_target, m_cam->m_up);
	Matrix wvp_mesh = pipline_mesh.get_pers_wvp_trans();

	m_shadow_map_tech->set_transformation(wvp_mesh, w_mesh);
	m_shadow_map_tech->set_light_wvp_trans(light_wvp_mesh);
	m_mesh->render(NULL);


}