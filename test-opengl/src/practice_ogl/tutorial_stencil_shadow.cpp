#include "tutorial_stencil_shadow.h"
#include "common/utils.h"

TutorialStencilShadow::TutorialStencilShadow()
{
	m_cam = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT);
	m_silhouette_tech = new SilhouetteDetectionTechnique();
	m_dir_light_tech = new DirectionLightTechnique();
	m_box = new VAOMesh();
}

TutorialStencilShadow::~TutorialStencilShadow()
{
	SAFE_DELETE(m_cam);
	SAFE_DELETE(m_silhouette_tech);
	SAFE_DELETE(m_box);
	SAFE_DELETE(m_dir_light_tech);
}

bool TutorialStencilShadow::init()
{
	m_proj_info.fov = m3dDegToRad(60);
	m_proj_info.height = WINDOW_HEIGHT;
	m_proj_info.width = WINDOW_WIDTH;
	m_proj_info.z_near = 1;
	m_proj_info.z_far = 50;

	m_cam->m_pos = Vector3(0, 5, -10);
	m_cam->m_target = Vector3(0, -5, 10);
	m_cam->m_up = Vector3(0, 1, 0);

	m_with_adjacencies = true;
	m_box->load_mesh("res/box.obj", m_with_adjacencies);

	if (!m_silhouette_tech->init()) {
		printf("ERROR: init silhouette technique\n");
	}
	if (m_dir_light_tech->init()) {
		m_dir_light_tech->enable();
		m_dir_light_tech->set_texture_unit(COLOR_TEXTURE_UNIT_INDEX);
		m_dir_light_tech->set_specular_power(5);
		m_dir_light_tech->set_material_specular_intensity(1);
	}
	else {
		printf("ERROR: init silhouette technique\n");
	}

	m_dir_light.ambiance_intensity = 0.3;
	m_dir_light.color = Vector3(1, 1, 0);
	m_dir_light.diffuse_intensity = 0.6;
	m_dir_light.direction = Vector3(-1, -1, 1);

	return false;
}

void TutorialStencilShadow::render_scene_callback(float width, float height, float time)
{
	render_pass(time / 10);
}

void TutorialStencilShadow::key_callback(int key, int scancode, int action, int mods)
{
}

void TutorialStencilShadow::cursor_position_callback(double x, double y)
{
}

void TutorialStencilShadow::render_pass(float time)
{
	Pipline pipline;
	pipline.set_rotation(0, time, 0);
	pipline.set_camera_info(m_cam->m_pos, m_cam->m_target, m_cam->m_up);
	pipline.set_pers_proj_info(m_proj_info);
	Matrix wvp = pipline.get_pers_wvp_trans(), world_trans = pipline.get_world_trans();


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	// dir light pass
	m_dir_light_tech->enable();
	m_dir_light_tech->set_eye_pos(m_cam->m_pos);
	m_dir_light_tech->set_light(m_dir_light);
	m_dir_light_tech->set_transformation(wvp, world_trans);
	m_box->render(NULL, GL_TRIANGLES_ADJACENCY);
	// silhouette pass
	glLineWidth(2);
	m_silhouette_tech->enable();
	m_silhouette_tech->set_light_pos(m_cam->m_pos);
	m_silhouette_tech->set_world_mat(world_trans);
	m_silhouette_tech->set_wvp_mat(wvp);
	m_box->render(NULL, GL_TRIANGLES_ADJACENCY);
}

