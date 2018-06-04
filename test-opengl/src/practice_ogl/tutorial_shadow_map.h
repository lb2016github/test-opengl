#ifndef _TUTORIAL_SHADOW_MAP_H
#define _TUTORIAL_SHADOW_MAP_H

#include "common/i_callbacks.h"
#include "common/camera.h"
#include "common/mesh.h"
#include "common/pipline.h"
#include "common/technique.h"
#include "common/light.h"
#include <vector>
#include "common/shadow_map_fbo.h"
#include "common/skybox.h"

class ShadowMapTechnique : public SpotLightTechnique {
public:
	ShadowMapTechnique();

	bool init();
	void set_light_wvp_trans(M3DMatrix44f light_wvp);
	void set_shadow_map_tex_unit(unsigned int tex_idx);
protected:
	virtual void init_shader_path();

private:
	GLuint m_light_wvp_location;
	GLuint m_sampler_shadow_map_location;
};

class TutorialShadowMap : public ICallback {
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

	M3DVector3f m_mesh_pos;
	M3DVector3f m_mesh_rot;
	M3DVector3f m_mesh_scale;

	ShadowMapTechnique* m_shadow_map_tech;

	SpotLight m_spot_light;

	ShadowMapFBO* m_shadow_map_fbo;

	Texture* m_test_tex;

	Skybox* m_skybox;
};

#endif // !_TUTORIAL_SHADOW_MAP_H
