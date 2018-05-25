#include "tutorial_point_light.h"
#include "const.h"
#include "glfw/include/glfw3.h"

#define FIElD_WIDTH 10
#define FIELD_DEPTH 20 

class PlaneMesh : public SimpleMesh {

public:
	bool load_mesh(const std::string& filename) {
		// init vertexes
		std::vector<Vertex> vertices = {
			Vertex(0.0f, 0.0f, 0.0f, 0, 0),
			Vertex(0.0f, 0.0f, FIELD_DEPTH, 0, 1),
			Vertex(FIElD_WIDTH, 0, 0, 1, 0),
			Vertex(FIElD_WIDTH, 0, FIELD_DEPTH, 1, 1)
		};
		std::vector<unsigned int> indices = {
			0, 1, 2,
			1, 3, 2,
		};

		calc_normal(vertices, indices);

		m_mesh_ent.init(vertices, indices);

		// init texture
		m_tex = new Texture(GL_TEXTURE_2D, "res/test.png");
		m_tex->load();

		return true;
	}
};

bool TutorialPointLight::init() {
	m_proj_info.fov = m3dDegToRad(60);
	m_proj_info.height = WINDOW_HEIGHT;
	m_proj_info.width = WINDOW_WIDTH;
	m_proj_info.z_near = 1;
	m_proj_info.z_far = 50;

	M3DVector3f pos, target, up;
	pos[0] = 5, pos[1] = 1, pos[2] = -3;
	target[0] = 0, target[1] = 0, target[2] = 1;
	up[0] = 0, up[1] = 1, up[2] = 0;
	m_cam = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, pos, target, up);

	m_mesh = new PlaneMesh();
	m_mesh->load_mesh("");

	m_tech = new PointLightTechnique();
	if (!m_tech->init()) {
		printf("Error: init technique\n");
	}
	m_tech->enable();
	m_tech->set_texture_unit(0);

	m3dLoadVector3(m_direction_light.color, 1, 1, 1);
	m_direction_light.ambiance_intensity = 0;
	m_direction_light.diffuse_intensity = 0.01f;
	m3dLoadVector3(m_direction_light.direction, 1, -1, 0);

	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	return true;
}

// 渲染场景
void TutorialPointLight::render_scene_callback(float width, float height, float time) {
	glClear(GL_COLOR_BUFFER_BIT);

	m_cam->on_render_cb();

	M3DMatrix44f wvp, w;
	m_pipline.set_world_pos(0, 0, 1);
	//m_pipline.set_scale(2);
	//m_pipline.set_rotation(0, time / 5, 0);
	m_pipline.set_camera_info(m_cam->m_pos, m_cam->m_target, m_cam->m_up);

	m_pipline.set_pers_proj_info(m_proj_info);
	m_pipline.get_pers_wvp_trans(wvp);
	m_pipline.get_world_trans(w);

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

	m_tech->set_direction_light(m_direction_light);
	m_tech->set_eye_position(m_cam->m_pos);
	m_tech->set_point_lights(pl);
	m_tech->set_specular_parameter(0, 0);
	m_tech->set_transformation(wvp, w);

	m_mesh->render();
}

// 键盘回调
void TutorialPointLight::key_callback(int key, int scancode, int action, int mods) {
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

// 光标移动回调
void TutorialPointLight::cursor_position_callback(double x, double y) {
	m_cam->on_mouse_move(x, y);
}