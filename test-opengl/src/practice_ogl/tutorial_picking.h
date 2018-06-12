#ifndef _TUTORIAL_PICKING_H
#define _TUTORIAL_PICKING_H
#include "common/i_callbacks.h"
#include "common/camera.h"
#include "common/mesh.h"
#include "common/technique.h"
#include "common/light.h"
#include <vector>
#include "common/pipline.h"


class TutorialPicking : public ICallback {
public:
	TutorialPicking();
	~TutorialPicking();

	bool init();

	// 渲染场景
	void render_scene_callback(float width, float height, float time);

	// 键盘回调
	void key_callback(int key, int scancode, int action, int mods);

	// 光标移动回调
	void cursor_position_callback(double x, double y);

	// 鼠标点击回调
	void mouse_button_callback(int button, int action, int mods);

private:
	void picking_pass();
	void render_pass();

private:
	Camera * m_cam;
	PersProjInfo m_proj_info;

	IMesh* m_mesh;
	IMesh* m_quad;
	
	PickingTechnique* m_picking_technique;
	SimpleColorTechnique* m_simple_color_technique;
	SimpleShowTechnique* m_simple_show_technique;

	PickingTexture* m_picking_texture;

	std::vector<M3DVector3f> m_world_pos;

	struct {
		bool is_pressed;
		int x;
		int y;
	}m_left_mouse_button;
};

#endif // !_TUTORIAL_PICKING_H
