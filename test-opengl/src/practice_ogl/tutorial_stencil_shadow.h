#ifndef _TUTORIAL_STENCIL_SHADOW_H
#define _TUTORIAL_STENCIL_SHADOW_H

#include "common/i_callbacks.h"
#include "common/camera.h"
#include "common/technique.h"
#include "common/pipline.h"
#include "common/mesh.h"

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

private:
	Camera * m_cam;
	PersProjInfo m_proj_info;
	SilhouetteDetectionTechnique* m_silhouette_tech;
	DirectionLightTechnique* m_dir_light_tech;
	DirectionLight m_dir_light;


	IMesh* m_box;
	bool m_with_adjacencies;

};

#endif // !_TUTORIAL_STENCIL_SHADOW_H
