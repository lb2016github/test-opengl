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

	// ��Ⱦ����
	void render_scene_callback(float width, float height, float time);

	// ���̻ص�
	void key_callback(int key, int scancode, int action, int mods);

	// ����ƶ��ص�
	void cursor_position_callback(double x, double y);

private:
	void ds_geom_pass(float time);
	void ds_begin_light_pass();
	void ds_dir_light_pass();
	void ds_rend_pass();

private:
	Camera * m_cam;
	PersProjInfo m_proj_info;

	VAOMesh* m_mesh;
	VAOMesh* m_quad;
	DSGeometryTechnique* m_tech;
	DSDirLightTechnique* m_dir_tech;

	GBuffer m_gbuffer;

	DirectionLight m_dir_light;
};

#endif // !_TUTORIAL_DEFERRED_SHADING
