#ifndef _TUTORIAL_SPOT_LIGHT_H
#define _TUTORIAL_SPOT_LIGHT_H
#include "common/i_callbacks.h"
#include "common/camera.h"
#include "common/mesh.h"
#include "common/pipline.h"
#include "common/technique.h"
#include "common/light.h"
#include <vector>


class TutorialSpotLight : public ICallback {
public:
	bool init();

	// 渲染场景
	void render_scene_callback(float width, float height, float time);

	// 键盘回调
	void key_callback(int key, int scancode, int action, int mods);

	// 光标移动回调
	void cursor_position_callback(double x, double y);

private:
	Camera * m_cam;
	PersProjInfo m_proj_info;
	Pipline m_pipline;

	IMesh* m_mesh;
	Mesh* m_box;

	SpotLightTechnique* m_tech;

	DirectionLight m_direction_light;
};

#endif // !_TUTORIAL_SPOT_LIGHT_H
