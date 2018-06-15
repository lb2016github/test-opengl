#include "tutorial_tessellation.h"
#include "common/const.h"
#include "glfw3/glfw3.h"
#include "common/pipline.h"
#include "common/utils.h"

#define FIElD_WIDTH 10
#define FIELD_DEPTH 20 


TutorialTessellation::TutorialTessellation() {
	m_cam = NULL;
	m_tech = NULL;
	m_color_tex = NULL;
	m_height_tex = NULL;
	m_mesh = NULL;
}
TutorialTessellation::~TutorialTessellation() {
	SAFE_DELETE(m_cam);
	SAFE_DELETE(m_tech);
	SAFE_DELETE(m_color_tex);
	SAFE_DELETE(m_height_tex);
	SAFE_DELETE(m_mesh);
}

bool TutorialTessellation::init() {
	m_proj_info.fov = m3dDegToRad(60);
	m_proj_info.height = WINDOW_HEIGHT;
	m_proj_info.width = WINDOW_WIDTH;
	m_proj_info.z_near = 1;
	m_proj_info.z_far = 50;

	M3DVector3f pos, target, up;
	pos[0] = 0, pos[1] = 1, pos[2] = -5;
	target[0] = 0, target[1] = -0.2, target[2] = 1;
	up[0] = 0, up[1] = 1, up[2] = 0;
	m_cam = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, pos, target, up);

	m_tech = new TessellationTechnique();
	m_tech->init();
	m_tech->enable();
	m_tech->set_tex_color_index(COLOR_TEXTURE_UNIT_INDEX);
	m_tech->set_tex_height_map_index(HEIGHT_TEXTURE_UNIT_INDEX);

	m_color_tex = new Texture(GL_TEXTURE_2D, "res/diffuse.jpg");
	m_height_tex = new Texture(GL_TEXTURE_2D, "res/heightmap.jpg");
	m_color_tex->load();
	m_height_tex->load();

	m_mesh = new Mesh();
	m_mesh->load_mesh("res/quad2.obj");

	m_height_factor = 1;
	m_is_wire_frame = false;

	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE | GL_DEPTH_TEST);

	return true;
}

// 渲染场景
void TutorialTessellation::render_scene_callback(float width, float height, float time) {
	m_cam->on_render_cb();

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	m_color_tex->bind(COLOR_TEXTURE_UNIT);
	m_height_tex->bind(HEIGHT_TEXTURE_UNIT);

	Pipline pipline;
	pipline.set_world_pos(0, 0, 0);
	pipline.set_scale(2);
	pipline.set_camera_info(m_cam->m_pos, m_cam->m_target, m_cam->m_up);
	pipline.set_pers_proj_info(m_proj_info);

	M3DMatrix44f world, vp;
	pipline.get_world_trans(world);
	pipline.get_vp_trans(vp);

	m_tech->enable();
	m_tech->set_eye_pos(m_cam->m_pos);
	m_tech->set_height_factor(m_height_factor);
	m_tech->set_vp(vp);
	m_tech->set_world(world);

	m_mesh->render(NULL, GL_PATCHES);
}

// 键盘回调
void TutorialTessellation::key_callback(int key, int scancode, int action, int mods) {
	m_cam->on_keyboard(key);

	if (action != GLFW_PRESS) return;

	switch (key)
	{
	case GLFW_KEY_UP:
		m_height_factor += 0.1;
		break;
	case GLFW_KEY_DOWN:
		m_height_factor -= 0.1;
		break;
	case GLFW_KEY_Z:
		m_is_wire_frame = !m_is_wire_frame;
		if (m_is_wire_frame) glPolygonMode(GL_FRONT, GL_LINE);
		else glPolygonMode(GL_FRONT, GL_FILL);
		break;
	default:
		break;
	}
}

// 光标移动回调
void TutorialTessellation::cursor_position_callback(double x, double y) {
	m_cam->on_mouse_move(x, y);
}
