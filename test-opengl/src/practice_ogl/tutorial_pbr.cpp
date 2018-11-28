#include "tutorial_pbr.h"
#include "common/utils.h"

TutorialPBR::TutorialPBR(): m_technique(NULL), m_cam(NULL), m_model(NULL)
{

}

TutorialPBR::~TutorialPBR()
{
	SAFE_DELETE(m_technique);
	SAFE_DELETE(m_model);
	SAFE_DELETE(m_cam);
}

bool TutorialPBR::init()
{
	m_proj_info.fov = m3dDegToRad(60);
	m_proj_info.height = WINDOW_HEIGHT;
	m_proj_info.width = WINDOW_WIDTH;
	m_proj_info.z_near = 1;
	m_proj_info.z_far = 500;

	Vector3 pos(0, 5, -5), target(0, -1, 1), up(0, 1, 0);
	m_cam = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, pos, target, up);

	m_technique = new PBRTechnique();
	if (!m_technique->init()) {
		printf("Init PBRTechnique failed\n");
	}
	m_model = new Mesh();
	m_model->load_mesh("res/KSR-29 sniper rifle new_obj.obj");

	m_point_lights[0].diffuse_intensity = 0.5;
	m_point_lights[0].color = Vector3(1, 1, 1);
	m_point_lights[0].position = Vector3(0, 2, 0);
	m_point_lights[0].atten.linear = 0.1;
	m_point_lights[0].atten.exp = 0.5;
	m_point_lights[0].atten.constant = 1;

	m_textures.push_back(new Texture(GL_TEXTURE_2D, "res/textures/KSR29sniperrifle_Base_Color.jpg"));
	m_textures[0]->load();
	m_textures.push_back(new Texture(GL_TEXTURE_2D, "res/textures/KSR29sniperrifle_Normal_OpenGL.jpg"));
	m_textures[1]->load();
	m_textures.push_back(new Texture(GL_TEXTURE_2D, "res/textures/KSR29sniperrifle_Roughness.jpg"));
	m_textures[2]->load();
	m_textures.push_back(new Texture(GL_TEXTURE_2D, "res/textures/KSR29sniperrifle_Roughness.jpg"));
	m_textures[3]->load();
	m_textures.push_back(new Texture(GL_TEXTURE_2D, "res/textures/KSR29sniperrifle_low_Material.005_AmbientOcclusion.jpg"));
	m_textures[4]->load();

	return true;
}

void TutorialPBR::render_scene_callback(float width, float height, float time)
{
	// pipline
	Pipline pipline;
	pipline.set_camera_info(m_cam->m_pos, m_cam->m_target, m_cam->m_up);
	pipline.set_world_pos(0, 0, 0);
	pipline.set_scale(1);
	pipline.set_rotation(0, 0, 0);
	pipline.set_pers_proj_info(m_proj_info);
	// set technique
	m_technique->enable();
	m_technique->set_cam_position(m_cam->m_pos);
	m_technique->set_mvp_mtx(pipline.get_pers_wvp_trans());
	m_technique->set_world_mtx(pipline.get_world_trans());
	m_technique->set_point_lights(1, m_point_lights);
	m_technique->set_tex_albedo(ALBEDO_TEXTURE_UNIT_INDEX);
	m_technique->set_tex_metalness(METALNESS_TEXTURE_UNIT_INDEX);
	m_technique->set_tex_roughness(ROUGHNESS_TEXTURE_UNIT_INDEX);
	m_technique->set_tex_roughness(AO_TEXTURE_UNIT_INDEX);

	m_textures[0]->bind(ALBEDO_TEXTURE_UNIT);
	m_textures[1]->bind(NORMAL_TEXTURE_UNIT);
	m_textures[2]->bind(ROUGHNESS_TEXTURE_UNIT);
	m_textures[3]->bind(METALNESS_TEXTURE_UNIT);
	m_textures[4]->bind(AO_TEXTURE_UNIT);

	// render
	m_model->render(NULL);
}

void TutorialPBR::key_callback(int key, int scancode, int action, int mods)
{

}

void TutorialPBR::cursor_position_callback(double x, double y)
{

}

