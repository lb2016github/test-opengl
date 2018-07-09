#ifndef _TUTORIAL_NORMAL_MAP_H
#define _TUTORIAL_NORMAL_MAP_H

#include "common/i_callbacks.h"
#include "common/camera.h"
#include "common/mesh.h"
#include "common/pipline.h"
#include "common/technique.h"
#include "common/light.h"
#include <vector>
#include "common/shadow_map_fbo.h"
#include "common/skybox.h"

class TutorialNormalMap : public ICallback {
public:
	bool init();

	// 渲染场景
	void render_scene_callback(float width, float height, float time);

	// 键盘回调
	void key_callback(int key, int scancode, int action, int mods);

	// 光标移动回调
	void cursor_position_callback(double x, double y);

protected:
	void shadow_map_pass();
	void render_pass();

private:
	Camera * m_cam;
	PersProjInfo m_proj_info;

	Mesh* m_plane;
	Mesh* m_mesh;

	Vector3 m_mesh_pos;
	Vector3 m_mesh_rot;
	Vector3 m_mesh_scale;

	LightingTechnique* m_shadow_map_tech;

	SpotLight m_spot_light;

	ShadowMapFBO* m_shadow_map_fbo;

	Texture* m_test_tex;

	Texture* m_normal_map;
	Texture* m_up_map;
	Texture* m_normal_tex;

	Skybox* m_skybox;
};

#endif // !_TUTORIAL_NORMAL_MAP_H
