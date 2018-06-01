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

class ShadowMapTechnique : public Technique {
public:
	ShadowMapTechnique();

	bool init();
	void set_wvp_trans(M3DMatrix44f wvp);
	void set_texture_unit(unsigned int tex_unit);
private:
	GLuint m_wvp_location;
	GLuint m_shadow_map_location;
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

	ShadowMapTechnique* m_shadow_map_tech;

	SpotLight m_spot_light;

	ShadowMapFBO* m_shadow_map_fbo;
};

#endif // !_TUTORIAL_SHADOW_MAP_H
