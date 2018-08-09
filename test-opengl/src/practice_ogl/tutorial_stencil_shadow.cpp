#include "tutorial_stencil_shadow.h"
#include "common/utils.h"

TutorialStencilShadow::TutorialStencilShadow()
{
	m_cam = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT);
	m_null_tech = new NullTechnique();
	m_ssv_tech = new StencilShadowVolumeTechnique();
	m_point_light_tech = new PointLightTechnique();
	m_box = new VAOMesh();
	m_floor = new VAOMesh();
	m_floor_texure = new Texture(GL_TEXTURE_2D, "res/test.png");
}

TutorialStencilShadow::~TutorialStencilShadow()
{
	SAFE_DELETE(m_cam);
	SAFE_DELETE(m_null_tech);
	SAFE_DELETE(m_ssv_tech);
	SAFE_DELETE(m_box);
	SAFE_DELETE(m_floor);
	SAFE_DELETE(m_point_light_tech);
	SAFE_DELETE(m_floor_texure);
}

bool TutorialStencilShadow::init()
{
	m_proj_info.fov = m3dDegToRad(60);
	m_proj_info.height = WINDOW_HEIGHT;
	m_proj_info.width = WINDOW_WIDTH;
	m_proj_info.z_near = 1;
	m_proj_info.z_far = 50;

	m_cam->m_pos = Vector3(0, 20, -7);
	m_cam->m_target = Vector3(0, -1, 1);
	m_cam->m_up = Vector3(0, 1, 0);

	m_with_adjacencies = true;
	m_box->load_mesh("res/box.obj", m_with_adjacencies);
	m_floor->load_mesh("res/quad.obj", m_with_adjacencies);
	m_box_orientation.position = Vector3(0, 2, 0);
	m_floor_orientation.scale = Vector3(10, 10, 10);
	m_floor_orientation.rotation = Vector3(m3dDegToRad(90), 0, 0);
	m_floor_texure->load();

	m_point_light.color = Vector3(1, 1, 1);
	m_point_light.position = Vector3(0, 10, 0);

	if (!m_null_tech->init()) {
		printf("ERROR: init null technique\n");
	}
	if (!m_ssv_tech->init()) {
		printf("ERROR: init stencil shadow volume technique\n");
	}
	if (m_point_light_tech->init()) {
		m_point_light_tech->enable();
		m_point_light_tech->set_texture_unit(COLOR_TEXTURE_UNIT_INDEX);
		m_point_light_tech->set_specular_parameter(0, 0);
	}
	else {
		printf("ERROR: init silhouette technique\n");
	}

	glEnable(GL_DEPTH_TEST | GL_CULL_FACE | GL_STENCIL_TEST);
	glFrontFace(GL_CW);

	return false;
}

void TutorialStencilShadow::render_scene_callback(float width, float height, float time)
{
	render_pass(time / 10);
	//m_cam->on_render_cb();
}

void TutorialStencilShadow::key_callback(int key, int scancode, int action, int mods)
{
	//m_cam->on_keyboard(key);
}

void TutorialStencilShadow::cursor_position_callback(double x, double y)
{
	//m_cam->on_mouse_move(x, y);
}

void TutorialStencilShadow::render_pass(float time)
{
	glDepthMask(GL_TRUE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	m_box_orientation.rotation[1] = time;
	glDrawBuffer(GL_NONE);
	glEnable(GL_DEPTH_TEST);
	// render scene to depth
	render_to_depth();
	// render stencil volume to stencil
	glDepthMask(GL_FALSE);
	glEnable(GL_STENCIL_TEST);
	glEnable(GL_DEPTH_CLAMP);
	glDisable(GL_CULL_FACE);
	glStencilFunc(GL_ALWAYS, 0, 0xff);
	glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
	glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);
	render_shadow_volume_to_stencil();
	// render diffuse scene
	glDrawBuffer(GL_BACK);
	glEnable(GL_CULL_FACE);
	glDisable(GL_DEPTH_CLAMP);
	glStencilFunc(GL_EQUAL, 0, 0xff);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	m_point_light.ambiance_intensity = 0;
	m_point_light.diffuse_intensity = 0.8;
	render_scene();
	// render ambiance scene
	glDisable(GL_STENCIL_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);
	m_point_light.ambiance_intensity = 0.2;
	m_point_light.diffuse_intensity = 0;
	render_scene();
	glDisable(GL_BLEND);
}

void TutorialStencilShadow::render_to_depth()
{
	m_null_tech->enable();
	Pipline pipline;
	pipline.set_camera_info(m_cam->m_pos, m_cam->m_target, m_cam->m_up);
	pipline.set_pers_proj_info(m_proj_info);

	pipline.set_world_pos(m_box_orientation.position);
	pipline.set_rotation(m_box_orientation.rotation);
	Matrix wvp = pipline.get_pers_wvp_trans();
	m_null_tech->set_wvp_mtx(wvp);
	m_box->render(NULL, GL_TRIANGLES_ADJACENCY);

	pipline.set_world_pos(m_floor_orientation.position);
	pipline.set_rotation(m_floor_orientation.rotation);
	pipline.set_scale(m_floor_orientation.scale);
	wvp = pipline.get_pers_wvp_trans();
	m_null_tech->set_wvp_mtx(wvp);
	m_floor->render(NULL, GL_TRIANGLES_ADJACENCY);
}

void TutorialStencilShadow::render_shadow_volume_to_stencil()
{
	m_ssv_tech->enable();
	Pipline pipline;
	pipline.set_camera_info(m_cam->m_pos, m_cam->m_target, m_cam->m_up);
	pipline.set_pers_proj_info(m_proj_info);
	pipline.set_world_pos(m_box_orientation.position);
	pipline.set_rotation(m_box_orientation.rotation);
	Matrix vp = pipline.get_vp_trans();
	Matrix world_trans = pipline.get_world_trans();

	m_ssv_tech->set_light_position(m_point_light.position);
	m_ssv_tech->set_vp_mtx(vp);
	m_ssv_tech->set_world_mtx(world_trans);
	m_box->render(NULL, GL_TRIANGLES_ADJACENCY);
}

void TutorialStencilShadow::render_scene()
{
	m_point_light_tech->enable();
	m_point_light_tech->set_eye_position(m_cam->m_pos);
	m_point_light_tech->set_point_light(m_point_light);

	Pipline pipline;
	pipline.set_camera_info(m_cam->m_pos, m_cam->m_target, m_cam->m_up);
	pipline.set_pers_proj_info(m_proj_info);

	pipline.set_world_pos(m_box_orientation.position);
	pipline.set_rotation(m_box_orientation.rotation);
	Matrix wvp = pipline.get_pers_wvp_trans();
	Matrix world_trans = pipline.get_world_trans();
	m_point_light_tech->set_transformation(wvp, world_trans);
	m_box->render(NULL, GL_TRIANGLES_ADJACENCY);

	pipline.set_world_pos(m_floor_orientation.position);
	pipline.set_rotation(m_floor_orientation.rotation);
	pipline.set_scale(m_floor_orientation.scale);
	wvp = pipline.get_pers_wvp_trans();
	world_trans = pipline.get_world_trans();
	m_point_light_tech->set_transformation(wvp, world_trans);
	m_floor_texure->bind(COLOR_TEXTURE_UNIT);
	m_floor->render(NULL, GL_TRIANGLES_ADJACENCY);

}
