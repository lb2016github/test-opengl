#ifndef _TUTORIAL_PBR_H
#define _TUTORIAL_PBR_H

#include "common/i_callbacks.h"
#include "common/camera.h"
#include "common/technique.h"
#include <vector>
#include "common/pipline.h"
#include "common/mesh.h"

class TutorialPBR : public ICallback {
public:
	TutorialPBR();
	~TutorialPBR();

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

	PBRTechnique* m_technique;
	Mesh* m_model;

	PointLight m_point_lights[1];
	std::vector<Texture*> m_textures;
};

#endif // !_TUTORIAL_PBR_H
