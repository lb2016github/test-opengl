#ifndef _TUTORIAL_DEFERRED_SHADING
#define _TUTORIAL_DEFERRED_SHADING

#include "common/i_callbacks.h"
#include "common/camera.h"
#include "common/technique.h"
#include <vector>
#include "common/mesh.h"
#include "common/pipline.h"
#include "common/g_buffer.h"

class TutorialDeferredShading : public ICallback {
public:
	TutorialDeferredShading();
	~TutorialDeferredShading();

	bool init();

	// 渲染场景
	void render_scene_callback(float width, float height, float time);

	// 键盘回调
	void key_callback(int key, int scancode, int action, int mods);

	// 光标移动回调
	void cursor_position_callback(double x, double y);

private:
	void init_lights();
	void init_tech();
	void init_box_positions();
	float calc_light_sphere_distance(PointLight& light);
	void ds_geom_pass(float time);
	void ds_begin_light_pass();
	void ds_dir_light_pass();
	void ds_point_light_pass();
	void ds_rend_pass();

private:
	Camera * m_cam;
	PersProjInfo m_proj_info;

	VAOMesh m_box;
	VAOMesh m_quad;
	VAOMesh m_sphere;
	DSGeometryTechnique* m_tech;
	DSDirLightTechnique* m_dir_tech;
	DSPointLightTechnique* m_point_tech;

	GBuffer m_gbuffer;

	DirectionLight m_dir_light;
	PointLight m_point_lights[3];
	Vector3 m_box_positions[5];

	bool m_with_adjacencies;
};

#endif // !_TUTORIAL_DEFERRED_SHADING
