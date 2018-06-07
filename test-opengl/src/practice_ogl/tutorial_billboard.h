#ifndef _TUTORIAL_BILLBOARD_H
#define _TUTORIAL_BILLBOARD_H

#include "common/i_callbacks.h"
#include "common/camera.h"
#include "common/technique.h"
#include <vector>
#include "common/skybox.h"
#include "common/billboard_list.h"

class TutorialBillboard : public ICallback {
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

	Skybox* m_skybox;
	BillboardList* m_billboard_list;
};

#endif // !_TUTORIAL_BILLBOARD_H
