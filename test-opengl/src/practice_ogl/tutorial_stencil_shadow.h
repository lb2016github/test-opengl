#ifndef _TUTORIAL_STENCIL_SHADOW_H
#define _TUTORIAL_STENCIL_SHADOW_H

#include "common/i_callbacks.h"
#include "common/camera.h"
#include "common/technique.h"
#include "common/pipline.h"
#include "common/mesh.h"

struct Orientation {
	Vector3 position;
	Vector3 rotation;
	Vector3 scale;
};

class TutorialStencilShadow : public ICallback {
public:
	TutorialStencilShadow();
	~TutorialStencilShadow();

	bool init();

	// 渲染场景
	void render_scene_callback(float width, float height, float time);

	// 键盘回调
	void key_callback(int key, int scancode, int action, int mods);

	// 光标移动回调
	void cursor_position_callback(double x, double y);

protected:
	void render_pass(float time);
	void render_to_depth();
	void render_shadow_volume_to_stencil();
	void render_scene();

private:
	Camera * m_cam;
	PersProjInfo m_proj_info;
	NullTechnique* m_null_tech;
	StencilShadowVolumeTechnique* m_ssv_tech;
	PointLightTechnique* m_point_light_tech;

	PointLight m_point_light;
	IMesh* m_box;
	IMesh* m_floor;
	bool m_with_adjacencies;

	Orientation m_box_orientation;
	Orientation m_floor_orientation;

	Texture* m_floor_texure;

};

#endif // !_TUTORIAL_STENCIL_SHADOW_H
