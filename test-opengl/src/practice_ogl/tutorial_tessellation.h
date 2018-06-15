#ifndef _TUTORIAL_TESSELLATION_H
#define _TUTORIAL_TESSELLATION_H

#include "common/i_callbacks.h"
#include "common/camera.h"
#include "common/technique.h"
#include <vector>
#include "common/skybox.h"
#include "common/billboard_list.h"

class TutorialTessellation : public ICallback {
public:
	TutorialTessellation();
	~TutorialTessellation();

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
	TessellationTechnique* m_tech;
	Texture* m_color_tex;
	Texture* m_height_tex;
	IMesh* m_mesh;


	float m_height_factor;
	bool m_is_wire_frame;
};

#endif // !_TUTORIAL_TESSELLATION_H
