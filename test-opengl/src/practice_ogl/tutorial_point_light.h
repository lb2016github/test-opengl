#ifndef _TUTORIAL_POINT_LIGHT_H
#define _TUTORIAL_POINT_LIGHT_H
#include "i_callbacks.h"
#include "camera.h"
#include "mesh.h"
#include "pipline.h"
#include "technique.h"
#include "light.h"
#include <vector>


class TutorialPointLight : public ICallback {
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
	
	Mesh* m_mesh;
	PointLightTechnique* m_tech;

	DirectionLight m_direction_light;
};

#endif // !_TUTORIAL_POINT_LIGHT_H
